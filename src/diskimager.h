#ifndef DISKIMAGER_H
#define DISKIMAGER_H

#include <QObject>
#include <QJsonObject>

#ifdef Q_OS_WIN
    #include "windowsdiskmanager.h"
#else
    #include "linuxdiskmanager.h"
#endif
/********************************************************************
 * DiskImager is the master object for the ODroid Flash Tool imaging
 * functions. Diskimager will allow the read and write of images to
 * and from a local drive. The class will contain #ifdef SYSTEM to
 * determine what diskamanger to load (linux vs windows).
 *
 *******************************************************************/

class OdroidFlashManager;
class DiskImager : public QObject
{
    Q_OBJECT
public:
    explicit DiskImager(QObject *parent = 0);
    void initializeDiskImager();
    /***************************************************************************
     * DiskImager State System:
     *  Each state has a purpose and allows the GUI to respect internal events.
     *
     * READY_STATE - in ready state the Window Disk Manager is ready to mount,
     *    and then begin reading or writing.
     *
     * WRITING_STATE - an active write (lock) is in progress.
     *
     * READING_STATE - an active read (lock) is in progress.
     *
     * WAITING_STATE - waiting to mount, read, or write.
     *
     * MOUNTED_STATE - mounted state allows DiskImager to mount an image
     *   without the purpose of reading or writing.
     *
     ***************************************************************************/
    enum ImagerState{READY_STATE = 0, WRITING_STATE, READING_STATE, WAITING_STATE, MOUNTED_STATE};
    Q_ENUM(ImagerState)
signals:
    void  errorOccured(ImagerState state,QString error);
    void  readCompleted(int percentage, QString write_speed);

public slots:
    void  queryBootIni();
    void  requestWriteImage(QString image_path, QString write_path,const OdroidFlashManager & odf);
    void  requestReadImage(QString read_path, QString image_path,const OdroidFlashManager & odf);
    void  generateMd5Hash(QString image_path);
    //void    verifyImage();
    void  onClosingApplication();
private:
#ifdef Q_OS_WIN
    WindowsDiskManager * mDiskManager;
#else
    LinuxDiskManager  *  mDiskManager;
#endif
    QJsonObject    mGUIDTable;
    ImagerState    mState;
private slots:
    void  diskImageError(QString error);
};

#endif // DISKIMAGER_H
