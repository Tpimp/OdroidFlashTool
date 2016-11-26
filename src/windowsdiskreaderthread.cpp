#include "windowsdiskreaderthread.h"
#include <QDebug>
#include "partitionfactory.h"
#include <QBuffer>
#include <fcntl.h>
#include <QDataStream>
ReaderThread::ReaderThread(QString disk_path, QString image_path, bool read_partitions, bool verify_after) : mDiskPath(disk_path), mImagePath(image_path),
    mReadOnlyPartitions(read_partitions), mVerify(verify_after)
{}



void ReaderThread::calculateSpeed()
{
    if(mOldDifference == 0)
        mOldDifference = mBytesWritten - mLastBytesWritten;
    else
        mOldDifference = (mOldDifference + mBytesWritten - mLastBytesWritten)/qreal(2);
    qreal megs(qreal(mOldDifference)/1048576);
    qreal  percent(qreal(mBytesWritten)/qreal(mBytesToWrite));
    mLastBytesWritten = mBytesWritten;
    int seconds((mBytesToWrite  - mBytesWritten)/mOldDifference); // to be on the safe side
    QString time_left;
    if(seconds > 59)
    {
        int minutes = seconds/60;
        seconds %= 60;
        time_left = QString("%1 minute").arg(minutes);
        if(minutes > 1)
            time_left.append('s');
        if(seconds > 0)
        {
            time_left.append( QString(" and %1 second").arg(seconds));
            if(seconds > 1)
                time_left.append('s');
        }
    }
    else
    {
        if(seconds > 0)
        {
            time_left.append( QString("%1 second").arg(seconds));
            if(seconds > 1)
                time_left.append('s');
        }
    }
    emit updateReadSpeed(percent * 100, SPEED_STR_VAL.arg(megs,0,'f',2),time_left);
}


void ReaderThread::performDiskCopy()
{
    quint64 bytes_read(0);
    mBytesToWrite = 0;
    for(int i(0); i < mDiskMap.first().writeChunks.length()-1; i++) // calculate total write length
    {
        if(i == 0)
        {
            const WriteChunk &sector(mDiskMap.first().writeChunks.at(i));
            mBytesToWrite += (sector.second * mReadSectorSize);
        }
        else{
            const WriteChunk &sector(mDiskMap.first().writeChunks.at(i));
            const WriteChunk &last_sector(mDiskMap.first().writeChunks.at(i-1));
            quint64 range = (sector.first - (last_sector.first + last_sector.second));
            mBytesToWrite += range;
            quint64 partition_size = (sector.second * mReadSectorSize);
            mBytesToWrite += partition_size;
        }
    }
    for (mBytesWritten =0; mBytesWritten < mBytesToWrite;)
    {
        bytes_read = mVolumeFile.read(mDataSector,mReadSectorSize);
        if (bytes_read == 0)
        {
            QString error;
            WindowsDiskManager::removeLock(mVolumeHandle,error);

            //emit error(tr("Write Error") + " : REACHED A NULL SECTOR in Reading before expected end");
            return;
        }
        quint64 bytes_written = mImageFile.write(mDataSector,bytes_read);
        if(bytes_written < bytes_read)
        {
            qDebug() << "Didnt write all that was read";
            bytes_written += mImageFile.write(mDataSector + (sizeof(char) * (bytes_read - bytes_written)),bytes_read - bytes_written);
            if(bytes_written < bytes_read)
            {
                bytes_written += mImageFile.write(mDataSector + (sizeof(char) * (bytes_read - bytes_written)),bytes_read - bytes_written);
            }
        }
        else
        {
            bytes_written += bytes_read;
        }
        mBytesWritten += bytes_written;
        QGuiApplication::processEvents();
    }

}

void ReaderThread::performDiskCopyWithVerify()
{

}


void ReaderThread::performVolumeCopy()
{

    QByteArray FILE_CACHE;
    int bytes_cached(0);
    QString error_str;
    if(!WindowsDiskManager::unmountVolume(mVolumeHandle,mDiskPath.at(0).toLatin1(),error_str))
    {
        WindowsDiskManager::removeLock(mVolumeHandle,error_str);
        emit finishedReadingImage(mImagePath);
        return;
    }

    //DiskDescriptorInfo   info(mDiskMap.values().first());
    DiskDescriptorInfo disk_info;
    DiskDescriptorInfo volume_info;
    //start by copying the mbr!
    int count(0);

    mBytesToWrite = 0;

    uchar * destination_ptr(nullptr);


    for(int i(0); i < mDiskMap.first().writeChunks.length()-1; i++) // calculate total write length
    {
        if(i == 0)
        {
            const WriteChunk &sector(mDiskMap.first().writeChunks.at(i));
            mBytesToWrite += (sector.second * mReadSectorSize);
        }
        else{
            const WriteChunk &sector(mDiskMap.first().writeChunks.at(i));
            const WriteChunk &last_sector(mDiskMap.first().writeChunks.at(i-1));
            quint64 range = (sector.first - (last_sector.first + last_sector.second)) * mReadSectorSize;
            mBytesToWrite += range;
            quint64 partition_size = (sector.second * mReadSectorSize);
            mBytesToWrite += partition_size;
        }
    }
   // mBytesToWrite += ((mDiskMap.writeChunks.first().first * mReadSectorSize)); // account for MBR/EBR data
    qint64 READ_CACHE_SIZE =(mBytesToWrite)/8;
    if(READ_CACHE_SIZE > 14485760)
        READ_CACHE_SIZE = 14485760;

    mImageFile.resize(mBytesToWrite);
    mImageFile.flush();
    mImageFile.close();
    mImageFile.open(QIODevice::ReadWrite);

    QFile  disk_handle;
    bool opened_raw_disk = disk_handle.open(_open_osfhandle(int(mRawDiskHandle),_O_RDONLY),QIODevice::ReadOnly);
    if(!opened_raw_disk)
        return;
    QDataStream disk_stream(&disk_handle);

    disk_handle.seek(0); // end mbr


    // seek to the beginning of linux section, read 512 and fill EBR_TABLE see what happens

    quint64 file_pos(0);
    quint64 bytes_read = 1;
    for(mBytesWritten = 0; mBytesWritten < mBytesToWrite && bytes_read > 0; )
    {
        bytes_read = disk_stream.readRawData(mDataSector,mReadSectorSize);
        FILE_CACHE.append(mDataSector,bytes_read);
        if((bytes_cached = FILE_CACHE.size()) >= READ_CACHE_SIZE)
        {
            destination_ptr = mImageFile.map(file_pos,bytes_cached);
            file_pos+= bytes_cached;
            if(!destination_ptr)
            {
                // failed to map memory
                qDebug() << mImageFile.errorString();
                return;
            }
            memcpy(destination_ptr,FILE_CACHE.data(),bytes_cached);
            mImageFile.unmap(destination_ptr);
           // mImageFile.write(FILE_CACHE);
            bytes_cached = 0;
            FILE_CACHE.clear();
        }
        mBytesWritten += bytes_read;
        QGuiApplication::processEvents();
    }
    if((bytes_cached = FILE_CACHE.size()) > 0)
    {
        destination_ptr = mImageFile.map(file_pos,bytes_cached);
        file_pos+= bytes_cached;
        if(!destination_ptr)
        {
            // failed to map memory
            qDebug() << mImageFile.errorString();
            return;
        }
        memcpy(destination_ptr,FILE_CACHE.data(),bytes_cached);
        mImageFile.unmap(destination_ptr);
       // mImageFile.write(FILE_CACHE);
        bytes_cached = 0;
        FILE_CACHE.clear();
    }
    mImageFile.flush();
}


void ReaderThread::performVolumeCopyWithVerify()
{

}


ReaderThread::~ReaderThread()
{
    if(mDataSector)
        delete [] mDataSector;
    mDataSector = nullptr;
}
