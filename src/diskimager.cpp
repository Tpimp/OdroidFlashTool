#include "diskimager.h"


#ifdef Q_OS_WIN
    #include "windowsdiskmanager.h"
#else
    #include "linuxdiskmanager.h"
#endif
DiskImager::DiskImager(QObject *parent) : QObject(parent), mGUIDTable()
{

}



void DiskImager::initializeDiskImager()
{
    #ifdef Q_OS_WIN
        mDiskManager = new WindowsDiskManager(this);
    #else
        mDiskManager = new LinuxDiskManager(this);
    #endif
}


void DiskImager::queryBootIni()
{

}

void DiskImager::queryDiskLocations()
{

}

void DiskImager::requestReadImage(QString read_path, QString image_path)
{

}


void DiskImager::requestWriteImage(QString image_path, QString write_path)
{

}
