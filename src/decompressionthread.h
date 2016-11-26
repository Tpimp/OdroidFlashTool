#ifndef DECOMPRESSIONTHREAD_H
#define DECOMPRESSIONTHREAD_H

#include <QObject>
#include <QThread>
#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include <QProcess>
#include <QDebug>
#include <QStringList>
#include <QGuiApplication>
#include <QRegExp>
#include <QDir>

class DecompressionThread : public QThread
{
    Q_OBJECT
public:
    DecompressionThread(QString inFile, QString pwd, QString temporary);
    void run() Q_DECL_OVERRIDE {

        // Run Some gaurd statements to check decompression is configured correctly

        QFile   archive_file(mInputFile); // Check if the archive even exists
        if(!archive_file.exists())
        {
            emit error(BAD_INPUT_DEC,"Archive not found: " + mInputFile);
            emit finishedDecompression();
            return;
        }

        QString program_path(mPWD + "/3rdparty/7zip/7zr.exe");  // Check can locate decompression process
        QFile  process_bin(program_path);
        if(!process_bin.exists())
        {
            emit error(BAD_LOCATE_DEC_BIN,"Decompression binary not found: " + program_path);
            emit finishedDecompression();
            return;
        }

        QDir dir(mTemp);
        if(dir.exists()) // Check if decompress temporary directory exists?
        {
            qDebug() << "Decompression thread - overwriting existing data: " << mTemp;
        }
        else{
            //create the temp folder
            dir.mkdir(mTemp);
            if(!dir.exists()) // check if could create ie
            {
                qDebug() << "ERROR Could not create temporary directory.";
                emit error(BAD_OUTPUT_DEC,"Failed to create temporary directory.");
                emit finishedDecompression();
                return;
            }
        }

        // Assuming all checks complete begin the decompression process
        int optimal_thread_count = QThread::idealThreadCount();
        // Generate arguments
        QByteArray run_argument;
        run_argument.append("-o");
        run_argument.append(mTemp);
        QStringList arguments;
        arguments << "x";
        if(optimal_thread_count > 1) // enable multi-threading
        {
            optimal_thread_count = 2;
            QByteArray thread_arg;
            thread_arg.append("-mmt");
            QString thread_ct(QString::number(optimal_thread_count));
            thread_arg.append(thread_ct);
            arguments << thread_arg;
        }
        arguments << run_argument << "-bsp1" << "-y" << mInputFile;
        QString  str_zero = arguments.at(0);
        str_zero = str_zero.remove('\\');

        // Create the process
        QProcess* decompression = new QProcess();
        decompression->setWorkingDirectory(mPWD + "/3rdparty/7zip/"); // set pwd
        decompression->setReadChannel(QProcess::StandardOutput); // we want to get the progress on stdout

        // Start the process and wait for it to start
        decompression->start(program_path, arguments);
        decompression->waitForStarted();

        // First 7zr will give us the header
        int progress(0);
        QString prg_str;
        QString header;
        while(decompression->state() != QProcess::NotRunning && header.length() < 500)
        {
            QByteArray output = decompression->read(500);
            if(!output.isEmpty())
            {
                header.append(output);
            }
            else {
                decompression->waitForReadyRead(35);
            }
        }

        // "Cleanse the header" It is heavily formated to display in commandline
        header = header.replace(QRegExp("\\s*\r\n\\s*"),"\n");
        header = header.replace(QRegExp("\\s*\n\n\\s*"),"\r");
        header = header.replace(QRegExp("\\s*\n\\s*"),"\r");
        QStringList header_items = header.split("\r"); // any remainding '\r' represent "Data Chunks"
        QString export_name(header_items.last());
        export_name = export_name.section("",7);
        emit startedDecompress(export_name);
        qDebug() << "Exporting: " << export_name;
        //qDebug() << "Found " << header_items.length() << " Items in Header";    // Output the almost nicely chunked header data


        // Read progress from the process
        while(decompression->state() != QProcess::NotRunning && progress != 100)
        {
            QByteArray output = decompression->read(330); // might truncate name
            if(!output.isEmpty())
            {
                int index(0);
                bool parsing(true);
                prg_str.clear();
                int size = output.length();
                while(parsing && index < size) // parse the number out
                {
                    char byte(output.at(index));
                    if(isdigit(byte))  //found first digit
                    {
                        prg_str.append(byte);
                        index++;
                        byte = output.at(index);
                        if(isdigit(byte)) // found a second digit
                        {
                            prg_str.append(byte);
                            index++;
                            byte = output.at(index);
                            if(isdigit(byte)) // found a 3 digit number
                            {
                                prg_str.append(byte);
                            }
                        }
                        parsing = false; // digit parsed
                    }
                    index++;
                }
                if(progress != 100)
                {
                    progress = prg_str.toInt(); // parse the int from buffer
                    emit decompressionProgress(progress);
                }
            }
            decompression->waitForReadyRead(15); // check next progress
        }
        emit decompressionProgress(100);
        decompression->close();
        delete decompression;
        emit finishedDecompression(mInputFile.section("",0,mInputFile.length()-3)); // actually complete
    }
    enum DecompressError{BAD_INPUT_DEC = 0, BAD_OUTPUT_DEC, BAD_FORMAT_DEC, BAD_LOCATE_DEC_BIN };
    Q_ENUM(DecompressError)
signals:
    void error(DecompressError error_code, QString error_text);
    void startedDecompress(QString output_name);
    void decompressionProgress(qreal progress);
    void finishedDecompression(QString file_path = "");

private:
    QString        mInputFile;
    QString        mPWD;
    QString        mTemp;
};

#endif // DECOMPRESSIONTHREAD_H
