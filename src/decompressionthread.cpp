#include "decompressionthread.h"

DecompressionThread::DecompressionThread(QString inFile, QString outFile, QString tempDir, quint64 memLimitSize) :
    mInputFile(inFile),mOutputFile(outFile),mTempDir(tempDir),mMemLimitSize(memLimitSize),mCompressionStream(LZMA_STREAM_INIT)
{

}
