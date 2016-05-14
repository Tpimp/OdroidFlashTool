#ifndef DISKIMAGER_H
#define DISKIMAGER_H

#include <QObject>
#include <QJsonObject>
/********************************************************************
 * DiskImager is the master object for the ODroid Flash Tool imaing
 * functions. Diskimager will allow the read and write of images to
 * and from a local drive. The system will contain #ifdef SYSTEM to
 * determine what methods to use (linux vs windows).
 *
 *******************************************************************/


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

public slots:
    void  queryDiskLocations();
    void  queryBootIni();
    void  requestWriteImage(QString image_path, QString write_path);
    void  requestReadImage(QString read_path, QString image_path);

private:
    QObject * mDiskManager;
    QJsonObject    mGUIDTable;
};

#endif // DISKIMAGER_H