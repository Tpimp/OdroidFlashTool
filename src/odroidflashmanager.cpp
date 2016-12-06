#include "odroidflashmanager.h"
#include <QDir>
#include <QStorageInfo>
#include <QList>
#include <QDate>
#include "applicationsettings.h"


OdroidFlashManager::OdroidFlashManager(DiskManager *disk_manager,ApplicationSettings* settings, QString application_dir, QObject *parent) : QObject(parent),
    mDiskManager(disk_manager), mAppSettings(settings), mApplicationDir(application_dir)
{
    if(mDiskManager)
    {
        //connect(mDiskImager, &DiskImager::readCompleted, this, &OdroidFlashManager::readCompleted);
    }
    if(settings != nullptr)
    {
        connect(mAppSettings,&ApplicationSettings::settingsChanged,this,&OdroidFlashManager::updateProcessView);
    }
}

void OdroidFlashManager::requestBackupBootFile(QString boot_file_path, QString disk_path)
{
    if(!mDiskManager->checkDiskMounted(disk_path))
    {
        mCurrentProcess = READING_BOOT;
        mDiskManager->mountDisk(disk_path);
        return;
    }

    if(mCurrentProcess == WAITING_PROCESS)
    {
        mCurrentProcess = READING_BOOT;
        mDiskManager->loadBootFromDevice(disk_path);
    }
    else
        emit errorHappened(READING_BOOT,4,QStringLiteral("System Busy - Cannot read boot at this time."));
}


void OdroidFlashManager::startCompression(QString image_path)
{
    /*if(mCompressionThread)
        return;
    mCompressionThread = new CompressionThread(image_path,mAppSettings->workDirectory(), mApplicationDir + QStringLiteral("/3rdparty/7zip/7zr.exe"));
    connect(mCompressionThread, &CompressionThread::compressionProgress,this,&OdroidFlashManager::compressionCompleted);
    connect(mCompressionThread, &CompressionThread::finishedCompression,this,&OdroidFlashManager::compressionFinished);
    mRunningProcesses.append(COMPRESSING);
    mCompressionThread->start();
    emit processStarted(COMPRESSING);*/
}

void OdroidFlashManager::startDecompression(QString archive_path)
{
    /*if(mDecompressThread)
        return;
    mDecompressThread = new DecompressionThread(archive_path,mApplicationDir,mAppSettings->workDirectory());
    connect(mDecompressThread, &DecompressionThread::decompressionProgress,this,&OdroidFlashManager::decompressionComplete);
    connect(mDecompressThread, &DecompressionThread::finishedDecompression,this,&OdroidFlashManager::decompressionFinished);
    mRunningProcesses.append(DECOMPRESSING);
    mDecompressThread->start();
    emit processStarted(DECOMPRESSING);*/
}


void OdroidFlashManager::cancelCurrentOperations()
{
    /*switch(mCurrentProcess)
    {
        case DECOMPRESSING:
        {
            mDecompressThread->quit();
            mDecompressThread->exit(1);
            decompressionFinished("");
            break;
        }
        case COMPRESSING:
        {
            mCompressionThread->quit();
            mCompressionThread->exit(1);
            compressionFinished("");
            break;
        }
        case READING_IMAGE:
        {
            mReaderThread->quit();
            mReaderThread->exit(1);
            readingImageFinished("");
            break;
        }
        case WRITING_IMAGE:
        {
            mWriterThread->quit();
            mWriterThread->exit(1);
            writingImageFinished();
            break;
        }
        default: break;
    }
    mRunningProcesses.clear();*/
}

void OdroidFlashManager::loadBootFromFile(QString boot_file_path)
{

}

void OdroidFlashManager::loadBootFromDevice(QString disk_path)
{

}

//void OdroidFlashManager::compressionFinished(QString file_out)
//{
    /*disconnect(mCompressionThread, 0,this,0);
    mCompressionThread->deleteLater();
    mCompressionThread = nullptr;
    mRunningProcesses.removeAll(COMPRESSING);
    mCurrentTarget = file_out;
    emit processFinished(COMPRESSING,file_out);*/
//}

//void OdroidFlashManager::decompressionFinished(QString file_out)
//{
    /*disconnect(mDecompressThread, 0,this,0);
    mDecompressThread->deleteLater();
    mDecompressThread = nullptr;
    mCurrentSource = file_out;
    mRunningProcesses.removeAll(DECOMPRESSING);
    emit processFinished(DECOMPRESSING,file_out);*/
//}

QVariantList OdroidFlashManager::getCrumbPath(QString path)
{
   return mDiskManager->getCrumbPath(path);
}

QString OdroidFlashManager::getSystemRootName()
{
    return mDiskManager->getSystemRootName();
}

QString OdroidFlashManager::getSystemRootPath()
{
    return mDiskManager->getSystemRootPath();
}

QString OdroidFlashManager::getSystemRootUrl()
{
    return mDiskManager->getSystemRootUrl();
}


void OdroidFlashManager::queryMountedDevices()
{
    mDiskManager->queryDevices(mAppSettings->showNotReadyDisks());
}


//void OdroidFlashManager::readingImageFinished(QString filename)
//{
    /*disconnect(mReaderThread, 0,this,0);
    mReaderThread->deleteLater();
    mReaderThread = nullptr;
    mRunningProcesses.removeAll(READING_IMAGE);
    emit processFinished(READING_IMAGE, filename);*/
//}

//void OdroidFlashManager::restoreLastBootBackup(QString drive_path)
//{
    //writeBootIni(mAppSettings->lastBootIniBackup(),drive_path);
//}

void OdroidFlashManager::restoreBackupFile(QString disk_path)
{

}

void OdroidFlashManager::startProcedure()
{

}

void OdroidFlashManager::startImageWrite(QString root_path, QString image_path, bool should_verify, bool write_partitions)
{
   mDiskManager->startImageWrite(root_path,image_path,should_verify,write_partitions);
}

void OdroidFlashManager::startNextProcess()
{

}


void OdroidFlashManager::startReadImage(QString root_path, QString image_path, bool should_verify, bool read_partitions)
{
    mDiskManager->startReadImage(root_path,image_path,should_verify, read_partitions);
}



QVariantList OdroidFlashManager::temporaryCrumbs()
{

    return mDiskManager->temporaryCrumbs(mAppSettings->workDirectory());
}

QUrl OdroidFlashManager::temporaryUrl()
{
    return QUrl::fromLocalFile(mAppSettings->workDirectory());
}


void OdroidFlashManager::updateProcessView()
{
    // maintain correct process view
    if(mAppSettings->flashMode()) // if write mode
    {
        if(mRunningProcesses.contains(READING_IMAGE) || mRunningProcesses.contains(READING_IMAGE_VERIFY))
        {
            mRunningProcesses.clear();
            emit clearProcessList();
        }
        if(mAppSettings->useCompression()) // should decompress before write
        {
            if(!mRunningProcesses.contains(DECOMPRESSING))
            {
                if(mRunningProcesses.isEmpty())
                {
                    emit appendProcess("DECOMPRESS");
                    mRunningProcesses.append(DECOMPRESSING);
                }
                else
                {
                    emit insertProcess(0,"DECOMPRESS");
                    mRunningProcesses.insert(0,DECOMPRESSING);
                }
            }
            if(mAppSettings->deleteAfterDecompress())
            {
                if(mRunningProcesses.contains(DELETE_IMAGE))
                {
                    int indexd(-1);
                    if((indexd = mRunningProcesses.indexOf(DELETE_IMAGE)) >=0)
                    {
                        emit removeProcessAt(indexd);
                        mRunningProcesses.removeAll(DELETE_IMAGE);
                    }
                }
                emit appendProcess("DELETE IMG");
                mRunningProcesses.append(DELETE_IMAGE);
            }
            else
            {
                int indexd(-1);
                if((indexd = mRunningProcesses.indexOf(DELETE_IMAGE)) >=0)
                {
                    emit removeProcessAt(indexd);
                    mRunningProcesses.removeAll(DELETE_IMAGE);
                }
            }

        }
        else
        {
            int index(mRunningProcesses.indexOf(DECOMPRESSING));
            if(index >=0)
            {
                emit removeProcessAt(index);
                mRunningProcesses.removeAll(DECOMPRESSING);
            }
            int indexd(mRunningProcesses.indexOf(DELETE_IMAGE));
            if(indexd >=0)
            {
                emit removeProcessAt(indexd);
                mRunningProcesses.removeAll(DELETE_IMAGE);
            }

        }
        if(mAppSettings->verifyFlash()) // verify write
        {
            int index(mRunningProcesses.indexOf(WRITING_IMAGE));
            if(index >= 0)
            {
                emit removeProcessAt(index);
                mRunningProcesses.removeAt(index);
            }
            if(!mRunningProcesses.contains(WRITING_IMAGE_VERIFY))
            {
                if(mAppSettings->useCompression())
                {
                    emit insertProcess(1,"WRITE IMG(V)");
                    mRunningProcesses.insert(1,WRITING_IMAGE_VERIFY);
                }
                else
                {
                    emit insertProcess(0,"WRITE IMG(V)");
                    mRunningProcesses.insert(0,WRITING_IMAGE_VERIFY);
                }
            }
        }
        else
        {
            int index(mRunningProcesses.indexOf(WRITING_IMAGE_VERIFY));
            if(index >= 0)
            {
                emit removeProcessAt(index);
                mRunningProcesses.removeAt(index);
            }
            if(!mRunningProcesses.contains(WRITING_IMAGE))
            {
                if(mAppSettings->useCompression())
                {
                    emit insertProcess(1,"WRITE IMG");
                    mRunningProcesses.insert(1,WRITING_IMAGE);
                }
                else
                {
                    emit insertProcess(0,"WRITE IMG");
                    mRunningProcesses.insert(0,WRITING_IMAGE);
                }
            }
        }
        if(mAppSettings->writeBootIni())
        {
            if(!mRunningProcesses.contains(WRITING_BOOT))
            {
                if(mRunningProcesses.contains(DELETE_IMAGE))
                {
                    int delete_index = mRunningProcesses.indexOf(DELETE_IMAGE);
                    emit insertProcess(delete_index-1,"WRITE BOOT");
                    mRunningProcesses.insert(delete_index-1,WRITING_BOOT);
                }
                else
                {
                    emit appendProcess("WRITE BOOT");
                    mRunningProcesses.append(WRITING_BOOT);
                }
            }
        }
        else
        {
            int indexw(-1);
            if((indexw = mRunningProcesses.indexOf(WRITING_BOOT)) >=0)
            {
                emit removeProcessAt(indexw);
                mRunningProcesses.removeAll(WRITING_BOOT);
            }
        }

    }
    else // read mode
    {
        if(mRunningProcesses.contains(WRITING_IMAGE) || mRunningProcesses.contains(WRITING_IMAGE_VERIFY))
        {
            mRunningProcesses.clear();
            emit clearProcessList();
        }
        if(mAppSettings->verifyFlash())
        {
            if(mRunningProcesses.isEmpty())
            {
                emit appendProcess("READ IMG(V)");
                mRunningProcesses.append(READING_IMAGE_VERIFY);
            }
            else
            {
                int read_index(-1);
                int readv_index(-1);
                if((read_index = mRunningProcesses.indexOf(READING_IMAGE)) >= 0 ||  (readv_index = mRunningProcesses.indexOf(READING_IMAGE_VERIFY)) >= 0)
                {
                    if(read_index >= 0)
                    {
                        emit removeProcessAt(read_index);
                        mRunningProcesses.removeAll(READING_IMAGE);
                    }
                    if(readv_index >= 0)
                    {
                        emit removeProcessAt(readv_index);
                        mRunningProcesses.removeAll(READING_IMAGE_VERIFY);
                    }
                }
                emit insertProcess(0,"READ IMG(V)");
                mRunningProcesses.insert(0,READING_IMAGE_VERIFY);
            }
        }
        else
        {
            if(mRunningProcesses.isEmpty())
            {
                emit appendProcess("READ IMG");
                mRunningProcesses.append(READING_IMAGE);
            }
            else
            {
                int read_index(-1);
                int readv_index(-1);
                if((read_index = mRunningProcesses.indexOf(READING_IMAGE)) >= 0 ||  (readv_index = mRunningProcesses.indexOf(READING_IMAGE_VERIFY)) >= 0)
                {
                    if(readv_index >= 0)
                    {
                        emit removeProcessAt(readv_index);
                        mRunningProcesses.removeAll(READING_IMAGE_VERIFY);
                    }
                    if(read_index >= 0)
                    {
                        emit removeProcessAt(read_index);
                        mRunningProcesses.removeAll(READING_IMAGE);
                    }
                }
                emit insertProcess(0,"READ IMG");
                mRunningProcesses.insert(0,READING_IMAGE);
            }
        }
        if(mAppSettings->useCompression())
        {
            int index(-1);
            if((index = mRunningProcesses.indexOf(COMPRESSING)) >= 0)
            {
                emit removeProcessAt(index);
                mRunningProcesses.removeAll(COMPRESSING);
            }

            emit appendProcess("COMPRESS");
            mRunningProcesses.append(COMPRESSING);
            if(mAppSettings->deleteAfterCompress())
            {
                int index(-1);
                if((index = mRunningProcesses.indexOf(DELETE_IMAGE) ) >=0)
                {
                    emit removeProcessAt(index);
                    mRunningProcesses.removeAll(DELETE_IMAGE);
                }
                emit appendProcess("DELETE IMG");
                mRunningProcesses.append(DELETE_IMAGE);
            }
            else
            {
                int indexd(-1);
                if((indexd = mRunningProcesses.indexOf(DELETE_IMAGE)) >=0)
                {
                    emit removeProcessAt(indexd);
                    mRunningProcesses.removeAll(DELETE_IMAGE);
                }
            }

        }
        else
        {
            int cindex(-1);
            if((cindex = mRunningProcesses.indexOf(COMPRESSING)) >= 0)
            {
                    emit removeProcessAt(cindex);
                    mRunningProcesses.removeAll(COMPRESSING);
            }
            int indexd(-1);
            if((indexd = mRunningProcesses.indexOf(DELETE_IMAGE)) >=0)
            {
                emit removeProcessAt(indexd);
                mRunningProcesses.removeAll(DELETE_IMAGE);
            }

        }
        if(mAppSettings->writeBootIni())
        {
            if(mAppSettings->preserveBootIni())
            {
                if(mRunningProcesses.isEmpty())
                {
                    if(mAppSettings->preserveBootIni())
                    {
                        mRunningProcesses.append(READING_BOOT);
                        emit appendProcess("READ BOOT");
                    }
                    emit appendProcess("WRITE BOOT");
                    mRunningProcesses.append(WRITING_BOOT);
                }
                else
                {
                    emit insertProcess(0,"READ BOOT");
                    mRunningProcesses.insert(0,READING_BOOT);
                    emit insertProcess(1,"WRITE BOOT");
                    mRunningProcesses.insert(1,WRITING_BOOT);
                }
                emit appendProcess("RESTORE BOOT");
                mRunningProcesses.append(RESTORING_BOOT);
            }
            else
            {
                int restore(-1);
                int read(-1);
                int write(-1);
                if((restore = mRunningProcesses.indexOf(RESTORING_BOOT)))
                mRunningProcesses.removeAll(READING_BOOT);
                mRunningProcesses.removeAll(WRITING_BOOT);
                emit appendProcess("WRITE BOOT");
                mRunningProcesses.append(WRITING_BOOT);
            }

        }
        else
        {
            // fill in removal
        }

    }
}


void OdroidFlashManager::writeMemoryToBootFile(QString disk_path, QString device_format)
{

}



void OdroidFlashManager::writeBootFromFile(QString boot_ini_path, QString drive_path) // TODO: defer to diskimager
{
   // load boot_file
   QFile  boot_ini(boot_ini_path);
   boot_ini.open(QIODevice::ReadOnly);
   QByteArray   boot_settings(boot_ini.readAll());
   mDiskManager->overwriteBootFile(boot_settings,drive_path);
}


/*****************************************************************************
*  ## Start of private methods attached to DiskManager
******************************************************************************/

void OdroidFlashManager::processBootSettingRecieved(QByteArray& settings)
{

}

void OdroidFlashManager::processBootSettingsWritten(QString disk_path)
{

}

void OdroidFlashManager::processBootSettingsNotFound(int error, QString& error_str)
{

}
void OdroidFlashManager::processDiskError(DiskManager::ImagerState state,QString error)
{

}
void OdroidFlashManager::processDiskFinished(DiskManager::ImagerState state)
{

}

void OdroidFlashManager::processDiskMounted(QString disk_path)
{

}

void OdroidFlashManager::processDiskStarted(DiskManager::ImagerState state)
{

}

void OdroidFlashManager::processDiskUnmounted(QString disk_path)
{

}

void OdroidFlashManager::processFoundDevice(QString device, QString device_path)
{

}

//void OdroidFlashManager::writingImageFinished()
//{
//    disconnect(mWriterThread, 0,this,0);
//    mWriterThread->deleteLater();
//    mWriterThread = nullptr;
//    mRunningProcesses.removeAll(WRITING_IMAGE);
//    emit processFinished(WRITING_IMAGE);
//}
