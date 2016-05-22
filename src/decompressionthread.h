#ifndef DECOMPRESSIONTHREAD_H
#define DECOMPRESSIONTHREAD_H

#include <QObject>
#include <QThread>
#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include <QProcess>


class DecompressionThread : public QThread
{
    Q_OBJECT
public:
    DecompressionThread(QString inFile, QString outFile, QString pwd);
    void run() Q_DECL_OVERRIDE {


        QFile   archive_file(mInputFile); // create the file
        if(!archive_file.exists())
        {
            emit error(BAD_INPUT_DEC,"Archive not found: " + mInputFile);
            emit finishedDecompression();
            return;
        }
        QString program_path(mPWD + "/3rdparty/7zip/7zr.exe");
        QStringList arguments;
        arguments << "-d" << mInputFile << "-o" << mOutputFile;

        QProcess* decompression = new QProcess(this);

        decompression->start(program_path,arguments);
        decompression->waitForStarted();
        quint64  waitTime(archive_file.size()/100000);
        int progress(0);
        while(!decompression->waitForFinished(waitTime))
        {
            emit decompressionProgress(progress++);
        }
        emit decompressionProgress(100);

        // prepare file out
        decompression->close();
        delete decompression;
        emit finishedDecompression();
    }
    enum DecompressError{BAD_INPUT_DEC = 0, BAD_OUTPUT_DEC, BAD_FORMAT_DEC, FAILED_DEC_STREAM };
    Q_ENUM(DecompressError)
signals:
    void error(DecompressError error_code, QString error_text);
    void decompressionProgress(qreal progress);
    void finishedDecompression();

private:
    QString        mInputFile;
    QString        mOutputFile;
    QString        mPWD;

private slots:

};

#endif // DECOMPRESSIONTHREAD_H
