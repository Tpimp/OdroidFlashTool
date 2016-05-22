#include "compressionmanager.h"
#include <stdio.h>
#include "decompressionthread.h"
CompressionManager::CompressionManager(QObject *parent) : QObject(parent)
{

}

void CompressionManager::cleanUpDecompressor()
{
    delete mDecompressor;
    mDecompressor = nullptr;
}

void CompressionManager::decompressFile(QString archive_path, QString decompress_path)
{
    if(mDecompressor == nullptr)
    {
        mDecompressor = new DecompressionThread(archive_path, decompress_path, mDecompressTempDir);
        // make connections to decompression thread
        connect(mDecompressor, &DecompressionThread::finishedDecompression, this, &CompressionManager::cleanUpDecompressor);
        mDecompressor->start();
    }
}



CompressionManager::~CompressionManager()
{

}
