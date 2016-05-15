#ifndef DECOMPRESSIONTHREAD_H
#define DECOMPRESSIONTHREAD_H

#include <QObject>
#include <QThread>
#include "3rdparty/xz/include/lzma.h"
class DecompressionThread : public QThread
{
    Q_OBJECT
public:
    DecompressionThread(QString inFile, QString outFile, QString tempDir, quint64 memLimitSize = 0);
    void run() Q_DECL_OVERRIDE {
        // Begin the xz decompression
        lzma_action  action(LZMA_RUN);


        emit finishedDecompression();
    }
    enum DecompressError{BAD_INPUT_DEC = 0, BAD_OUTPUT_DEC, BAD_FORMAT_DEC };
    Q_ENUM(DecompressError)
signals:
    void error(DecompressError error_code, QString error_text);
    void finishedDecompression();

private:
    QString        mInputFile;
    QString        mOutputFile;
    QString        mTempDir;
    quint64        mMemLimitSize;
    lzma_stream    mCompressionStream;
};

#endif // DECOMPRESSIONTHREAD_H
