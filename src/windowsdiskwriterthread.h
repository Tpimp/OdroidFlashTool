#ifndef WINDOWSDISKWRITERTHREAD_H
#define WINDOWSDISKWRITERTHREAD_H


#include <QObject>
#include "windowsdiskmanager.h"
#include <QGuiApplication>
#include <QStorageInfo>
#include <QFile>
#include <io.h>
#include <Fcntl.h>
#include <QThread>

class WriterThread : public QThread
{
    Q_OBJECT
public:
    WriterThread(QString disk_path, QString image_path, bool write_partitions = false, bool verify_after = false);
    ~WriterThread();
    void run() Q_DECL_OVERRIDE
    {
        QString   image_root_drive = mImagePath.section("",0,3);
        QStorageInfo  write_drive(mDiskPath);
        QStorageInfo  image_drive(image_root_drive);
        QString error_str;


        // check that the image file exists
        mImageFile.setFileName(mImagePath);
        if(!mImageFile.exists())
        {
            //error image doesnt exist
            mDataSector = nullptr;
            emit finishedWritingImage();
            return;
        }
        if(!mImageFile.open(QIODevice::ReadOnly))
        {
            long attempts(0);
            bool opened = mImageFile.open(QIODevice::ReadOnly);
            while(!opened && attempts < 10)
            {
                sleep(10);
                attempts++;
                opened = mImageFile.open(QIODevice::ReadOnly);
            }
            //error failed to open image file for reading
            mDataSector = nullptr;
            emit finishedWritingImage();
            return;
        }


        mImageHandle = HANDLE(_get_osfhandle(mImageFile.handle()));
        if(mImageHandle == INVALID_HANDLE_VALUE)
        {
            //error opened file for writing but failed to get a valid handle
            mDataSector = nullptr;
            emit finishedWritingImage();
            return;
        }


        if(write_drive.rootPath().compare(image_drive.rootPath()) == 0)// check the drives are not on the same disk
        {

            //emit error(tr("Write Error") + " : " +tr("System cannot read and write image from the same disk/volume.")); // convert this error
            mDataSector = nullptr;
            emit finishedWritingImage();
            return;
        }

        // create a handle to the disk volume
        if(!WindowsDiskManager::getVolumeHandle(mDiskPath.at(0).toLatin1(),mVolumeFile,error_str,mVolumeHandle, QIODevice::WriteOnly))
        {
            mDataSector = nullptr;
            emit finishedWritingImage();
            return;
        }
        if(!WindowsDiskManager::lockVolume(mVolumeHandle,error_str))
        {
            mDataSector = nullptr;
            emit finishedWritingImage();
            return;
        }


        int disk_number;
        quint64 disk_size;
        mReadSectorSize = WindowsDiskManager::getDriveDetails(write_drive.rootPath().at(0).toLatin1(),mVolumeHandle,mRawDiskHandle,mDiskMap, disk_number,disk_size, error_str);
        if(mImageFile.size() > disk_size)// check that the image file is not larger than storage medium (writing 8GB image to 4GB microSD)
        {
            // error - write device does not have enough space

            mDataSector = nullptr;
            emit finishedWritingImage();
            return;
        }
        if(!WindowsDiskManager::getRawDiskHandle(mRawDiskHandle,disk_number,error_str, FILE_SHARE_READ | FILE_SHARE_WRITE))
        {

            mDataSector = nullptr;
            WindowsDiskManager::removeLock(mVolumeHandle,error_str);
            mImageFile.close();
            mVolumeHandle = INVALID_HANDLE_VALUE;
            mImageHandle = INVALID_HANDLE_VALUE;
            emit finishedWritingImage();
            return;
        }
        if(!WindowsDiskManager::unmountVolume(mVolumeHandle,mDiskPath.at(0).toLatin1(),error_str))
        {
            mDataSector = nullptr;
            WindowsDiskManager::removeLock(mVolumeHandle,error_str);
            emit finishedWritingImage();
            return;
        }

        mDataSector = new char[mReadSectorSize +1];
        mReadBufferSize = (64000); // attempt to read around about 100x what we need
        mDataSector[mReadSectorSize] = '\0';
        mBytesToWrite = mImageFile.size();
        // calculate bytes to write.

        // begin write
        if(mWriteOnlyPartitions)
        {
            if(mVerify)
            {
                performVolumeWriteWithVerify();
            }
            else
                performVolumeWrite();
        }
        else
        {
            if(mVerify)
            {
                performDiskWriteWithVerify();
            }
            else
                performDiskWrite();
        }
        // clean up and finish out
        delete [] mDataSector;
        mDataSector = nullptr;
        WindowsDiskManager::removeLock(mVolumeHandle,error_str);
        mImageFile.close();
        mVolumeFile.close();
        mVolumeHandle = INVALID_HANDLE_VALUE;
        mImageHandle = INVALID_HANDLE_VALUE;
        emit updateWriteSpeed(100, "Finished","");
        emit finishedWritingImage();
        QGuiApplication::processEvents();
    }
    enum WriteError{ ACCESS_PERMISSION_ERROR = 0, PATH_ERROR, CANNOT_CREATE_FILE_ERROR, OUT_OF_MEMORY_ERROR };
    Q_ENUMS(WriteError)
signals:
    void  startingWritePartition(QString volume_name, int current_write, int total_write);
    void  errorHappened(WriteError error_enum, QString error_str);
    void  updateWriteSpeed(int percentage, QString write_speed, QString time_left);
    void  finishedWritingImage();
public slots:

private:
    QString               mDiskPath;
    QString               mImagePath;
    HANDLE                mVolumeHandle;
    HANDLE                mImageHandle;
    HANDLE                mRawDiskHandle;
    QFile                 mImageFile;
    QFile                 mVolumeFile;
    QFile                 mDiskFile;
    bool                  mWriteOnlyPartitions;
    bool                  mVerify;
    char *                mDataSector = nullptr;
    quint16               mReadSectorSize;
    quint64               mReadBufferSize;
    quint64               mBytesWritten;
    quint64               mBytesToWrite;
    quint64               mLastBytesWritten = 0;
    qreal                 mOldDifference =0;
    DiskMap               mDiskMap;
    const QString         SPEED_STR_VAL ="%1MB/s";
    void                  performDiskWrite();
    void                  performVolumeWrite();
    void                  performDiskWriteWithVerify();
    void                  performVolumeWriteWithVerify();
private slots:
    void                  calculateSpeed();
};

#endif // WINDOWSDISKWRITERTHREAD_H
