#ifndef COMPRESSIONTHREAD_H
#define COMPRESSIONTHREAD_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QDebug>

class CompressionThread : public QThread
{
    Q_OBJECT
public:
    CompressionThread(QString image_name, QString temp_directory, QString tool_path, QStringList arguments, int compression_value = 8) :
        mArchiveName(image_name + ".xz"),mImageName(image_name), mToolPath(tool_path),mTemporaryDirectory(temp_directory),
        mCompressionValue(compression_value)
    {}
    enum CompressError{BAD_INPUT_COMP = 0, BAD_OUTPUT_COMP, BAD_FORMAT_COMP, BAD_LOCATE_COMP_BIN };
    Q_ENUM(CompressError)
    void run() Q_DECL_OVERRIDE
    {
        // Run Some gaurd statements to check decompression is configured correctly

        QFile   image_file(mImageName); // Check if the archive even exists
        if(!image_file.exists())
        {
            emit error(BAD_INPUT_COMP,"Image File not found: " + mImageName);
            emit finishedCompression();
            return;
        }

        QDir dir(mTemporaryDirectory);
        if(dir.exists()) // Check if decompress temporary directory exists?
        {
            qDebug() << "Compression thread - overwriting existing data: " << mTemporaryDirectory;
        }
        else{
            //create the temp folder
            dir.mkdir(mTemporaryDirectory);
            if(!dir.exists()) // check if could create ie
            {
                qDebug() << "ERROR Could not create temporary directory.";
                emit error(BAD_OUTPUT_COMP,"Failed to create temporary directory.");
                emit finishedCompression();
                return;
            }
        }
        QStringList arguments;
        QProcess* compression = new QProcess();
        QString program_path = mToolPath;  // Check can locate decompression process
        if(program_path.isEmpty())
        {
            program_path = "/3rdparty/7zip/7zr.exe";
            QFile  process_bin(program_path);
            if(!process_bin.exists())
            {
                emit error(BAD_LOCATE_COMP_BIN,"Decompression binary not found: " + program_path);
                emit finishedCompression();
                return;
            }
        }
            // convert Decompression logic to compression logic
//            // Assuming all checks complete begin the decompression process
//            int optimal_thread_count = QThread::idealThreadCount();
//            // Generate arguments
//            QByteArray run_argument;
//            run_argument.append("-o");
//            run_argument.append(mTemporaryDirectory);
//            arguments << "x";
//            if(optimal_thread_count > 1) // enable multi-threading
//            {

//                    optimal_thread_count = 2;
//                QByteArray thread_arg;
//                thread_arg.append("-mmt");
//                QString thread_ct(QString::number(optimal_thread_count));
//                thread_arg.append(thread_ct);
//                arguments << thread_arg;
//            }
//            arguments << run_argument << "-bsp1" << "-y" << mArchiveName;
//            QString  str_zero = arguments.at(0);
//            str_zero = str_zero.remove('\\');
//            compression->setWorkingDirectory(mToolPath);
//        }
        // Create the process

         // set pwd
        compression->setReadChannel(QProcess::StandardOutput); // we want to get the progress on stdout

        // Start the process and wait for it to start
        compression->start(program_path, arguments);
        compression->waitForStarted();

        // First 7zr will give us the header
        int progress(0);
        QString prg_str;
        QString header;
        while(compression->state() != QProcess::NotRunning && header.length() < 500)
        {
            QByteArray output = compression->read(500);
            if(!output.isEmpty())
            {
                header.append(output);
            }
            else {
                compression->waitForReadyRead(35);
            }
        }

        // "Cleanse the header" It is heavily formated to display in commandline
        header = header.replace(QRegExp("\s*\r\n\s*"),"\n");
        header = header.replace(QRegExp("\s*\n\n\s*"),"\r");
        header = header.replace(QRegExp("\s*\n\s*"),"\r");
        QStringList header_items = header.split("\r"); // any remainding '\r' represent "Data Chunks"
        QString export_name(header_items.last());
        export_name = export_name.section("",7);
        emit startedCompression(export_name);
        qDebug() << "Exporting: " << export_name;
        //qDebug() << "Found " << header_items.length() << " Items in Header";    // Output the almost nicely chunked header data


        // Read progress from the process
        while(compression->state() != QProcess::NotRunning && progress != 100)
        {
            QByteArray output = compression->read(330); // might truncate name
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
                    emit compressionProgress(progress);
                }
            }
            compression->waitForReadyRead(15); // check next progress
        }
        emit compressionProgress(100);
        compression->close();
        delete compression;
        emit finishedCompression(); // actually complete
    }

signals:
    void error(CompressError error, QString error_str );
    void compressionProgress(int progress);
    void startedCompression(QString archive_name);
    void finishedCompression();

private:
    QString     mArchiveName;
    QString     mImageName;
    QString     mTemporaryDirectory;
    QString     mToolPath;
    QStringList mArguments;
    int         mCompressionValue;
};

#endif // COMPRESSIONTHREAD_H
