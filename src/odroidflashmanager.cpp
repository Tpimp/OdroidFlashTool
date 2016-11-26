#include "odroidflashmanager.h"
#include <QDir>
#include <QStorageInfo>
#include <QList>
#include <QDate>
#include "applicationsettings.h"


OdroidFlashManager::OdroidFlashManager(DiskImager *dskimg,ApplicationSettings* settings, QString application_dir, QObject *parent) : QObject(parent),
    mDiskImager(dskimg), mAppSettings(settings), mApplicationDir(application_dir)
{
    if(mDiskImager)
    {
        //connect(mDiskImager, &DiskImager::readCompleted, this, &OdroidFlashManager::readCompleted);
    }
    if(settings != nullptr)
    {
        connect(mAppSettings,&ApplicationSettings::settingsChanged,this,&OdroidFlashManager::updateProcessView);
    }
}

void OdroidFlashManager::backupBootIni(QString boot_ini_path)
{
    QString drive_letter(boot_ini_path.section("",0,2));
    QStorageInfo  info(drive_letter);
    if(info.isValid() && info.isReady())
    {
        QFile boot_file(boot_ini_path);
        if(boot_file.exists())
        {
            boot_file.open(QIODevice::ReadOnly);
            QByteArray boot_data(boot_file.readAll());
            // dump to work directory
            QString path_to_backup(mAppSettings->workDirectory());
            path_to_backup.append(QDate::currentDate().toString());
            path_to_backup.append("-boot.ini");
            QFile backup_file(path_to_backup);
            if(backup_file.open(QIODevice::WriteOnly))
            {
                backup_file.write(boot_data);
                mAppSettings->setLastBackupBootIni(path_to_backup);
                emit backedUpBootIni();
            }
            else
                emit errorHappened(READING_BOOT,2,QStringLiteral("Failed to create backup file in work directory- ") + mAppSettings->workDirectory());
        }
        else
            emit errorHappened(READING_BOOT,0,QStringLiteral("Cannot locate boot.ini for file path- ") + boot_ini_path);
    }
    else
        emit errorHappened(READING_BOOT,1,QStringLiteral("Drive is not ready - check media storage in drive ") + drive_letter);
}


void OdroidFlashManager::startCompression(QString image_path)
{
    if(mCompressionThread)
        return;
    mCompressionThread = new CompressionThread(image_path,mAppSettings->workDirectory(), mApplicationDir + QStringLiteral("/3rdparty/7zip/7zr.exe"));
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
    mDecompressThread = new DecompressionThread(archive_path,mApplicationDir,mAppSettings->workDirectory());
    connect(mDecompressThread, &DecompressionThread::decompressionProgress,this,&OdroidFlashManager::decompressionComplete);
    connect(mDecompressThread, &DecompressionThread::finishedDecompression,this,&OdroidFlashManager::decompressionFinished);
    mRunningProcesses.append(DECOMPRESSING);
    mDecompressThread->start();
    emit processStarted(DECOMPRESSING);
}


void OdroidFlashManager::cancelCurrentOperations()
{
    switch(mCurrentProcess)
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
    mRunningProcesses.clear();
}

void OdroidFlashManager::compressionFinished(QString file_out)
{
    disconnect(mCompressionThread, 0,this,0);
    mCompressionThread->deleteLater();
    mCompressionThread = nullptr;
    mRunningProcesses.removeAll(COMPRESSING);
    mCurrentTarget = file_out;
    emit processFinished(COMPRESSING,file_out);
}

void OdroidFlashManager::decompressionFinished(QString file_out)
{
    disconnect(mDecompressThread, 0,this,0);
    mDecompressThread->deleteLater();
    mDecompressThread = nullptr;
    mCurrentSource = file_out;
    mRunningProcesses.removeAll(DECOMPRESSING);
    emit processFinished(DECOMPRESSING,file_out);
}

QVariantList OdroidFlashManager::getCrumbPath(QString path)
{
    QStringList crumbs = path.split('/',QString::SkipEmptyParts);
    QVariantList list;
    QString current_path("");
    foreach(const QString & str, crumbs)
    {
        QVariantList item;
        item.append(QVariant::fromValue(str));
        if(current_path.isEmpty())
            current_path.append(str);
        else
        {
            current_path.append('/');
            current_path.append(str);
        }
        item.append(QVariant::fromValue(QUrl::fromLocalFile(current_path)));
        list.append(QVariant::fromValue(item));
    }
    return list;
}

QString OdroidFlashManager::getSystemRootName()
{
    QStorageInfo  root(QStorageInfo::root());
    return root.displayName();
}

QString OdroidFlashManager::getSystemRootPath()
{
    QStorageInfo  root(QStorageInfo::root());
    return root.rootPath();
}

QString OdroidFlashManager::getSystemRootUrl()
{
    QStorageInfo  root(QStorageInfo::root());
    QString url("file:///");
    url.append(root.rootPath());
    return url;
}

void OdroidFlashManager::processReadErrors(ReaderThread::ReaderError error, QString error_str)
{

}

void OdroidFlashManager::processWriteErrors(WriterThread::WriteError error, QString error_str)
{

}

void OdroidFlashManager::queryMountedDevices()
{
    bool use_not_ready = mAppSettings->showNotReadyDisks();
    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes())
    {
        if (storage.isValid()  && (storage.isReady() || use_not_ready) && !storage.isRoot())
        {
            if (!storage.isReadOnly())
            {
                emit foundMountedDevice(storage.displayName(),storage.rootPath());
            }
        }
    }
}


void OdroidFlashManager::readingImageFinished(QString filename)
{
    disconnect(mReaderThread, 0,this,0);
    mReaderThread->deleteLater();
    mReaderThread = nullptr;
    mRunningProcesses.removeAll(READING_IMAGE);
    emit processFinished(READING_IMAGE, filename);
}

void OdroidFlashManager::restoreLastBootBackup(QString drive_path)
{
    writeBootIni(mAppSettings->lastBootIniBackup(),drive_path);
}


void OdroidFlashManager::startProcedure()
{

}

void OdroidFlashManager::startImageWrite(QString root_path, QString image_path, bool should_verify, bool write_partitions)
{
    if(mDiskImager == nullptr)
        return;

    if(mRunningProcesses.length() > 0)
        return; // something is running


    if(mAppSettings->shouldCompress())
    {
        mCurrentProcess = DECOMPRESSING;
        if(mAppSettings->verifyFlash())
        {
            mRunningProcesses.append(WRITING_IMAGE_VERIFY);
        }
        else
        {
            mRunningProcesses.append(WRITING_IMAGE);
        }
        startDecompression(mCurrentSource);
    }
    else
    {
        mWriterThread = new WriterThread(root_path,image_path,mAppSettings->usePartitionsOnly(),mAppSettings->verifyFlash());
        connect(mWriterThread, &WriterThread::startingWritePartition, this, &OdroidFlashManager::startedReadPartitions);
        connect(mWriterThread, &WriterThread::updateWriteSpeed, this, &OdroidFlashManager::writeCompleted);
        connect(mWriterThread, &WriterThread::errorHappened,this,&OdroidFlashManager::processWriteErrors);
        connect(mWriterThread, &WriterThread::finishedWritingImage,this,&OdroidFlashManager::writingImageFinished);
        if(mAppSettings->verifyFlash())
        {
            mCurrentProcess = WRITING_IMAGE_VERIFY;
            emit processStarted(WRITING_IMAGE_VERIFY);
        }
        else
        {

            mCurrentProcess = WRITING_IMAGE;
            emit processStarted(WRITING_IMAGE);
        }
    }

    if(mAppSettings->deleteAfterDecompress())
    {
        mRunningProcesses.append(DELETE_IMAGE);
    }

    mWriterThread->start(QThread::HighestPriority);
}

void OdroidFlashManager::startNextProcess()
{

}


void OdroidFlashManager::startReadImage(QString root_path, QString image_path, bool should_verify, bool read_partitions)
{
    if(mDiskImager == nullptr)
        return;

    if(mRunningProcesses.length() > 0)
        return; // something is running

    mReaderThread = new ReaderThread(root_path,image_path,read_partitions,should_verify);
    connect(mReaderThread, &ReaderThread::startingReadPartition, this, &OdroidFlashManager::startedReadPartitions);
    connect(mReaderThread, &ReaderThread::updateReadSpeed, this, &OdroidFlashManager::readCompleted);
    connect(mReaderThread, &ReaderThread::errorHappened,this,&OdroidFlashManager::processReadErrors);
    connect(mReaderThread, &ReaderThread::finishedReadingImage,this,&OdroidFlashManager::readingImageFinished);
    emit processStarted(READING_IMAGE);
    mReaderThread->start(QThread::HighestPriority);
}



QVariantList OdroidFlashManager::temporaryCrumbs()
{
    QString temp_path = mAppSettings->workDirectory();
    QStringList crumbs = temp_path.split('/',QString::SkipEmptyParts);
    QVariantList list;
    QString current_path("");
    foreach(const QString & str, crumbs)
    {
        QVariantList item;
        item.append(QVariant::fromValue(str));
        if(current_path.isEmpty())
            current_path.append(str);
        else
        {
            current_path.append('/');
            current_path.append(str);
        }
        item.append(QVariant::fromValue(QUrl::fromLocalFile(current_path)));
        list.append(QVariant::fromValue(item));
    }
    return list;
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


void OdroidFlashManager::writeBootIni(QString boot_ini_path, QString drive_path)
{
    QStorageInfo info(drive_path);
    if(info.isValid()  && info.isReady())
    {
        if(info.isReadOnly())
        {
            QFile boot_file(drive_path + "/boot/boot.ini");
            if(boot_file.exists())
            {
                if(boot_file.open(QIODevice::WriteOnly))
                {
                    QFile boot_ini(boot_ini_path);
                    if(boot_ini.exists())
                    {
                        boot_ini.open(QIODevice::ReadOnly);
                        QByteArray boot_data(boot_ini.readAll());
                        boot_file.write(boot_data);
                        emit wroteBootIni();
                    }
                    else
                       emit errorHappened(WRITING_BOOT,4,QStringLiteral("Cannot open boot.ini- ") + boot_ini_path);
                }
                else
                    emit errorHappened(WRITING_BOOT,3,QStringLiteral("Cannot open boot file for write- ") + boot_file.fileName());
            }
            else
                emit errorHappened(WRITING_BOOT,2,QStringLiteral("Cannot locate boot.ini on drive- ") + drive_path);
        }
        else
            emit errorHappened(WRITING_BOOT,1,QStringLiteral("Drive mounted as read only- ") + drive_path);
    }
    else
        emit errorHappened(WRITING_BOOT,0,QStringLiteral("Drive is invalid or not ready (Check media and remount)- ")+ drive_path);
}

void OdroidFlashManager::writingImageFinished()
{
    disconnect(mWriterThread, 0,this,0);
    mWriterThread->deleteLater();
    mWriterThread = nullptr;
    mRunningProcesses.removeAll(WRITING_IMAGE);
    emit processFinished(WRITING_IMAGE);
}
