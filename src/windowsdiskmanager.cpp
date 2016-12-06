#include "windowsdiskmanager.h"
#include <QDebug>
#include <QFile>
#include <io.h>
#include <QStorageInfo>
#include "partitionfactory.h"
#include <QUrl>

WindowsDiskManager::WindowsDiskManager(QObject* parent) : DiskManager(parent)
{

}

WindowsDiskManager::~WindowsDiskManager()
{

}

void WindowsDiskManager::cancelCurrentOperation()
{

}


QVariantList WindowsDiskManager::getCrumbPath(QString path)
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


QString WindowsDiskManager::getSystemRootPath()
{
    QStorageInfo  root(QStorageInfo::root());
    return root.rootPath();
}


QString WindowsDiskManager::getSystemRootName()
{
    QStorageInfo  root(QStorageInfo::root());
    return root.displayName();
}


QString WindowsDiskManager::getSystemRootUrl()
{
    QStorageInfo  root(QStorageInfo::root());
    QString url("file:///");
    url.append(root.rootPath());
    return url;
}


void WindowsDiskManager::loadBootFromDevice(QString drive_path)
{
    /*QString drive_letter(boot_ini_path.section("",0,2));
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
     */
    QByteArray settings;
    emit finishedReadingBootSettings(settings);

}

 void WindowsDiskManager::mountDisk(QString disk_path)
 {

 }

 void WindowsDiskManager::queryDevices(bool show_not_ready)
 {
     foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes())
     {
         if (storage.isValid()  && (storage.isReady() || show_not_ready) && !storage.isRoot())
         {
             if (!storage.isReadOnly())
             {
                 emit foundDevice(storage.displayName(),storage.rootPath());
             }
         }
     }
 }

 void WindowsDiskManager::readingImageFinished(QString filename)
 {

 }

 void WindowsDiskManager::startImageWrite(QString root_path, QString image_path, bool should_verify, bool write_partitions)
 {
    /* if(mDiskImager == nullptr)
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

     mWriterThread->start(QThread::HighestPriority);*/
 }

void WindowsDiskManager::startReadImage(QString root_path, QString image_path, bool should_verify, bool read_partitions)
{
     //TODO re-write StartReadImage
  /*  if(mDiskImager == nullptr)
        return;

    if(mRunningProcesses.length() > 0)
        return; // something is running

    mReaderThread = new ReaderThread(root_path,image_path,read_partitions,should_verify);
    connect(mReaderThread, &ReaderThread::startingReadPartition, this, &OdroidFlashManager::startedReadPartitions);
    connect(mReaderThread, &ReaderThread::updateReadSpeed, this, &OdroidFlashManager::readCompleted);
    connect(mReaderThread, &ReaderThread::errorHappened,this,&OdroidFlashManager::processReadErrors);
    connect(mReaderThread, &ReaderThread::finishedReadingImage,this,&OdroidFlashManager::readingImageFinished);
    emit processStarted(READING_IMAGE);
    mReaderThread->start(QThread::HighestPriority);*/
}



QVariantList WindowsDiskManager::temporaryCrumbs(QString temp_path)
{
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



void WindowsDiskManager::unmountDisk(QString disk_path)
{

}


void WindowsDiskManager::overwriteBootFile(QByteArray& boot_data,  QString drive_path) // TODO: defer to diskimager
{
   /* QStorageInfo info(drive_path);
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
        */
}

/****************************************************************************
 *Private Slots for internal Threads
 ****************************************************************************/
