#include "windowsdiskwriterthread.h"
#include <QDebug>
#include <QBuffer>
#include <QDataStream>
#include "partitionfactory.h"
#include <QElapsedTimer>
WriterThread::WriterThread(QString disk_path, QString image_path, bool write_partitions, bool verify_after) : mDiskPath(disk_path), mImagePath(image_path),
    mWriteOnlyPartitions(write_partitions), mVerify(verify_after)
{

}

void WriterThread::calculateSpeed()
{
    if(mOldDifference == 0)
        mOldDifference = mBytesWritten - mLastBytesWritten;
    else
        mOldDifference = (mOldDifference + mBytesWritten - mLastBytesWritten)/qreal(2);
    qreal megs(mOldDifference/1000000);
    qreal  percent(mBytesWritten);
    percent /= mBytesToWrite;
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
    emit updateWriteSpeed(percent * 100, SPEED_STR_VAL.arg(megs,0,'f',2),time_left);
}


void WriterThread::performDiskWrite()
{
   // DWORD   bytes_written(0);
    mLastBytesWritten = 0;
    mOldDifference = 0;
    quint64 bytes_read(0);
    quint64 bytes_written(0);

    EBR_TABLE  read_table;
    //bytes_read = mVolumeFile.read(read_data,512);


    QFile  disk_handle;
    bool opened_raw_disk = disk_handle.open(_open_osfhandle(int(mRawDiskHandle),0),QIODevice::WriteOnly);
    if(!opened_raw_disk)
        return;

    //disk_handle.seek(disk_info.writeChunks.first().first + (( + disk_info.writeChunks.first().second)  * mReadSectorSize)); // end mbr
    bytes_read = mImageFile.read(mDataSector,512);
    read_table.setData(mDataSector,bytes_read);
    mImageFile.seek(0);
    disk_handle.seek(0);
    QElapsedTimer write_timer;
    write_timer.start();
    qreal msecs_since;
    // Change algorithm to reading in large chunk of image file
    // write out cache chunk ReadSector chunks at a time
    mBytesToWrite = mImageFile.size();
    for (mBytesWritten =0; mBytesWritten < mBytesToWrite;)
    {
        bytes_read = mImageFile.read(mDataSector,mReadSectorSize);
        bytes_written = disk_handle.write(mDataSector,bytes_read);
        mBytesWritten += bytes_read;
        if((msecs_since = write_timer.elapsed()) > 1000)
        {
            if(mOldDifference == 0)
                mOldDifference = mBytesWritten - mLastBytesWritten;
            else
                mOldDifference = (mOldDifference + mBytesWritten - mLastBytesWritten)/qreal(2);
            qreal megs(mOldDifference/1048576);
            megs /= (msecs_since/1000);
            write_timer.restart();
            qreal  percent(mBytesWritten);
            percent /= mBytesToWrite;
            mLastBytesWritten = mBytesWritten;
            int seconds((mBytesToWrite  - mBytesWritten)/(megs*1048576)); // to be on the safe side
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
            emit updateWriteSpeed(percent * 100, SPEED_STR_VAL.arg(megs,0,'f',2),time_left);
            QGuiApplication::processEvents();
        }


    }
}

void WriterThread::performDiskWriteWithVerify()
{

}

void WriterThread::performVolumeWrite()
{

}


void WriterThread::performVolumeWriteWithVerify()
{

}

WriterThread::~WriterThread()
{
    if(mDataSector)
        delete [] mDataSector;
    mDataSector = nullptr;
}
