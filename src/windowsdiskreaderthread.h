#ifndef WINDOWSDISKREADERTHREAD_H
#define WINDOWSDISKREADERTHREAD_H

#include <QObject>
#include <QGuiApplication>
#include <QStorageInfo>
#include <QFile>
#include <io.h>
#include <QTimer>
#include <QThread>
#include <QDebug>
#include "windowsdiskutility.h"

using namespace std;
class ReaderThread : public QThread
{
    Q_OBJECT
public:
    ReaderThread(QString disk_path, QString image_path, bool read_partitions = false, bool verify_after = false);
    ~ReaderThread();
    void run() Q_DECL_OVERRIDE
    {
        if(mDiskPath[2].toLatin1() == '/')
            mDiskPath[2] = '\\';

        // Check the storage root path
        QString error_str;
        QStorageInfo  read_drive(mDiskPath);


        // Get Volume Handle
        if(!getVolumeHandle(mDiskPath.at(0).toLatin1(),mVolumeFile,error_str,mVolumeHandle,QIODevice::ReadWrite))
        {
            mDataSector = nullptr;
            emit finishedReadingImage(mImagePath);
            return;
        }



        int disk_number;
        mReadSectorSize = getDriveDetails(mDiskPath.at(0).toLatin1(),mVolumeHandle,mRawDiskHandle,mDiskMap,disk_number,mBytesToWrite,error_str);
        mReadBufferSize = (16*mReadSectorSize);
        mDataSector = new char[mReadBufferSize +1];
        mDataSector[mReadBufferSize ] = '\0';

        if(!lockVolume(mVolumeHandle,error_str))
        {
            emit finishedReadingImage(mImagePath);
            return;
        }
        // Prepare the image file
        mImageFile.setFileName(mImagePath);
        mImageFile.open(QIODevice::WriteOnly);

        // Add my own image file checks here

        mImageHandle = HANDLE(_get_osfhandle(mImageFile.handle()));
        if (mImageHandle == INVALID_HANDLE_VALUE)
        {
            delete [] mDataSector;
            mDataSector = nullptr;
            removeLock(mVolumeHandle,error_str);
            mImageFile.close();
            mVolumeHandle = INVALID_HANDLE_VALUE;
            //error(tr("Write Error") + " : Failed to lock volume " + mImagePath);
            emit finishedReadingImage(mImagePath);
            return;
        }

        if(mRawDiskHandle == INVALID_HANDLE_VALUE)
        {
            delete [] mDataSector;
            mDataSector = nullptr;
            removeLock(mVolumeHandle,error_str);
            mImageFile.close();
            mVolumeHandle = INVALID_HANDLE_VALUE;
            mImageHandle = INVALID_HANDLE_VALUE;
            emit finishedReadingImage(mImagePath);
            return;
        }

       /* quint64 file_size(0);
        if(!WindowsDiskManager::getFileSize(mImageFile,image_drive, file_size, mReadSectorSize,read_sectors,error_str))
        {
            delete [] mDataSector;
            mDataSector = nullptr;
            //error not enough space
            WindowsDiskManager::removeLock(mVolumeHandle,error_str);
            mImageFile.close();
            mReadFile.close();
            mVolumeHandle = INVALID_HANDLE_VALUE;
            mImageHandle = INVALID_HANDLE_VALUE;
            emit finishedReadingImage();
            return;
        }*/

        mSpeedTimer = new QTimer(this);
        connect(mSpeedTimer, &QTimer::timeout, this, &ReaderThread::calculateSpeed);
        mSpeedTimer->setSingleShot(false);
        mSpeedTimer->start(1000);

        // Determine if based on writing only partitions or grabbing whole disk - calculate correct read and write values
        if(mReadOnlyPartitions)
        {
            // ensure volume list is upto date
            if(mVerify)
            {
                performVolumeCopyWithVerify();
            }
            else
            {
                performVolumeCopy();
            }
        }
        else
        {
            if(mVerify)
            {
                performDiskCopyWithVerify();
            }
            else
            {
                performDiskCopy();
            }
        }
        delete [] mDataSector;
        mDataSector = nullptr;
        mSpeedTimer->stop();
        disconnect(mSpeedTimer,0,0,0);
        delete mSpeedTimer;
        removeLock(mVolumeHandle,error_str);
        mImageFile.close();
        mVolumeFile.close();
        mVolumeHandle = INVALID_HANDLE_VALUE;
        mImageHandle = INVALID_HANDLE_VALUE;
        emit updateReadSpeed(100, "Finished","");
        // clean up and finish out
        emit finishedReadingImage(mImagePath);
        QGuiApplication::processEvents();
    }
    enum ReaderError{ ACCESS_PERMISSION_ERROR = 0, PATH_ERROR, CANNOT_CREATE_FILE_ERROR, OUT_OF_MEMORY_ERROR };
    Q_ENUMS(ReaderError)
signals:
    void  startingReadPartition(QString volume_name, int current_read, int total_reads);
    void  errorHappened(ReaderError error_enum, QString error_str);
    void  updateReadSpeed(int percentage, QString read_speed, QString time_left);
    void  finishedReadingImage(QString file_name);
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
    bool                  mReadOnlyPartitions;
    bool                  mVerify;
    char *                mDataSector = nullptr;
    quint32               mReadSectorSize;
    quint32               mReadBufferSize;
    QTimer                *mSpeedTimer;
    quint64               mBytesWritten;
    quint64               mBytesToWrite = 0;
    quint64               mLastBytesWritten = 0;
    quint64               mOldDifference =0;
    const QString         SPEED_STR_VAL ="%1MB/s";
    void                  performDiskCopy();
    void                  performVolumeCopy();
    void                  performDiskCopyWithVerify();
    void                  performVolumeCopyWithVerify();
    DiskMap               mDiskMap;
private slots:
    void                  calculateSpeed();
};

#endif // WINDOWSDISKREADERTHREAD_H
