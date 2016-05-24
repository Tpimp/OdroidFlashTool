#include "odroidflashmanager.h"
#include <QDir>
#include <QStorageInfo>
#include <QList>

OdroidFlashManager::OdroidFlashManager(DiskImager *dskimg, QString application_dir, QObject *parent) : QObject(parent), mDiskImager(dskimg),
    mApplicationDir(application_dir)
{

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


void OdroidFlashManager::decompressionFinished()
{
    disconnect(mDecompressThread, NULL,this,NULL);
    mDecompressThread->deleteLater();
    mDecompressThread = nullptr;
    mRunningProcesses.removeAll(DECOMPRESSING);
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

void OdroidFlashManager::startCompression(QString image_path)
{

}


void OdroidFlashManager::startImageWrite(QString image_path)
{

}

void OdroidFlashManager::startReadImage(QString root_path)
{

}
