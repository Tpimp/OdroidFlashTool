#include "diskimager.h"


DiskImager::DiskImager(QObject *parent) : QObject(parent), mGUIDTable()
{

}

void DiskImager::generateMd5Hash(QString image_path)
{
    // TODO: Convert MD5 hash to a threaded method
    /*md5label->setText(tr("Generating..."));
    QApplication::processEvents();

    MD5 md5;

    // may take a few secs - display a wait cursor
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    // "digestFile" computes the md5 - display it in the textbox
    md5label->setText(md5.digestFile(filename));

    // redisplay the normal cursor
    QApplication::restoreOverrideCursor();*/
}


void DiskImager::initializeDiskImager()
{
    #ifdef Q_OS_WIN
//        mDiskManager = new WindowsDiskManager(this);
//        connect(mDiskManager, &WindowsDiskManager::error,this,&DiskImager::diskImageError);
//        connect(mDiskManager, &WindowsDiskManager::readCompleted,this,&DiskImager::readCompleted);
    #else
        mDiskManager = new LinuxDiskManager(this);
        connect(mDiskManager, &LinuxDiskManager::error,this,&DiskImager::diskImageError);
    #endif
}




void DiskImager::onClosingApplication()
{
    //ToDo convert closing application logic to a non-coupled method
   /* if (status == STATUS_READING)
    {
        if (QMessageBox::warning(NULL, tr("Exit?"), tr("Exiting now will result in a corrupt image file.\n"
                                                       "Are you sure you want to exit?"),
                                 QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            status = STATUS_EXIT;
        }
        event->ignore();
    }
    else if (status == STATUS_WRITING)
    {
        if (QMessageBox::warning(NULL, tr("Exit?"), tr("Exiting now will result in a corrupt disk.\n"
                                                       "Are you sure you want to exit?"),
                                 QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            status = STATUS_EXIT;
        }
        event->ignore();
    }*/
}

void DiskImager::diskImageError(QString error)
{

}

void DiskImager::queryBootIni()
{

}


void DiskImager::requestReadImage(QString read_path, QString image_path, const OdroidFlashManager &odf)
{
   // if(!mDiskManager)
   //     return;
   // WindowsDiskManagerState = READING_STATE;
    //mDiskManager->requestReadImage(read_path,image_path);
}


void DiskImager::requestWriteImage(QString image_path, QString write_path,const OdroidFlashManager &odf)
{

}
