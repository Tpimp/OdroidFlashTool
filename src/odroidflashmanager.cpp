#include "odroidflashmanager.h"
#include <QDir>
#include <QStorageInfo>
#include <QList>

OdroidFlashManager::OdroidFlashManager(DiskImager *dskimg, QString application_dir, QObject *parent) : QObject(parent), mDiskImager(dskimg),
    mApplicationDir(application_dir)
{

}


void OdroidFlashManager::startCompression(QString image_path)
{
    if(mCompressionThread)
        return;
    mCompressionThread = new CompressionThread(image_path,mTemporaryPath, mApplicationDir + "/3rdparty/7zip/7zr.exe");
    connect(mCompressionThread, &CompressionThread::compressionProgress,this,&OdroidFlashManager::compressionCompleted);
    connect(mCompressionThread, &CompressionThread::finishedCompression,this,&OdroidFlashManager::compressionFinished);
    mRunningProcesses.append(COMPRESSING);
    mCompressionThread->start();
    emit processStarted(COMPRESSING);
}

void OdroidFlashManager::startDecompression(QString archive_path)
{
    if(mDecompressThread)
        return;
    mDecompressThread = new DecompressionThread(archive_path,mApplicationDir,mTemporaryPath);
    connect(mDecompressThread, &DecompressionThread::decompressionProgress,this,&OdroidFlashManager::decompressionComplete);
    connect(mDecompressThread, &DecompressionThread::finishedDecompression,this,&OdroidFlashManager::decompressionFinished);
    mRunningProcesses.append(DECOMPRESSING);
    mDecompressThread->start();
    emit processStarted(DECOMPRESSING);

}

void OdroidFlashManager::cancelCurrentOperation()
{
    if(mRunningProcesses.contains(DECOMPRESSING))
    {
        mDecompressThread->quit();
        mDecompressThread->exit(1);
        decompressionFinished();
    }
}

void OdroidFlashManager::compressionFinished()
{
    disconnect(mCompressionThread, NULL,this,NULL);
    mCompressionThread->deleteLater();
    mCompressionThread = nullptr;
    mRunningProcesses.removeAll(COMPRESSING);
    emit processFinished(COMPRESSING);
}

void OdroidFlashManager::decompressionFinished()
{
    disconnect(mDecompressThread, NULL,this,NULL);
    mDecompressThread->deleteLater();
    mDecompressThread = nullptr;
    mRunningProcesses.removeAll(DECOMPRESSING);
    emit processFinished(DECOMPRESSING);
}



void OdroidFlashManager::queryMountedDevices()
{
    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
        if (storage.isValid() && storage.isReady() && !storage.isRoot()) {
            if (!storage.isReadOnly()) {
                emit foundMountedDevice(storage.displayName(),storage.rootPath());
            }
        }
    }
}



void OdroidFlashManager::startImageWrite(QString image_path)
{
    if(mDiskImager == nullptr)
        return;

    if(mRunningProcesses.length() > 0)
        return; // something is running
}

void OdroidFlashManager::startReadImage(QString root_path, QString image_path)
{
    if(mDiskImager == nullptr)
        return;

    if(mRunningProcesses.length() > 0)
        return; // something is running

    mDiskImager->requestReadImage(root_path,image_path);
}
