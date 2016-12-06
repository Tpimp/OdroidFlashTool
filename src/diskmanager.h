#ifndef DISKMANAGER_H
#define DISKMANAGER_H
#include <QObject>
#include <QVariantList>
class QUrl;
class QString;
class DiskManager : public QObject
{
    Q_OBJECT
public:
    /******************************************************************************************
     *  DiskManager - Interface for abstracting platform DiskManagers for ODFTool (C.A.D 2016)
     ******************************************************************************************
     * DiskManager State System:
     *  Each state has a purpose and allows the GUI to respect internal events.
     *
     * READY_STATE - in ready state the Window Disk Manager is ready to mount,
     *    and then begin reading or writing.
     *
     * WRITING_STATE - an active write (lock) is in progress.
     *
     * READING_STATE - an active read (lock) is in progress.
     *
     * MOUNTING_STATE - attempting to mount the disk
     *   - The DiskManager will also attempt to mount only one drive (personally)
     *  at a time. Tracking the mounted state using mMountedToDrive property -
     *  the api will also trigger mountedDisk and unmountedDisk signals.
     *
     * WAITING_STATE - waiting to mount, read, or write.
     *
     *******************************************************************************************/
    enum ImagerState{NOT_READY_STATE = 0, READY_STATE, WRITING_STATE, WRITINGB_STATE, READING_STATE, READINGB_STATE, MOUNTING_STATE, UNMOUNTING_STATE, QUERYING_STATE, WAITING_STATE};
    Q_ENUM(ImagerState)

signals:
    // Below are signals exposed to classes monitoring the DiskManagement (very often a threaded event)
    void  bootSettingsNotFound(int error, QString& error_str);
    void  errorOccured(ImagerState state,QString error);   // Important for anything concerned with errors
    void  finishedReadingBootSettings(QByteArray& settings); // Got your boot settings (called before processFinsihed(READINGB))
    void  finishedWritingBootSettings(QString drive_path);   // Set your boot settings (called before processFinished(WRITINGB))
    void  foundDevice(QString device, QString device_path);  // in response to query devices // might run for a bit
    void  mountedDisk(QString disk_path);
    void  processCompleted(int percentage, QString speed);
    void  processFinished(ImagerState state);  // Finished some state - what next? - READY if mounted, WAITING if not
    void  processStarted(ImagerState state);   // Hey! Im going to start doing something now, if you care
    void  unmountedDisk(QString disk_path);    // All finished with the unmount process on disk drive (disk_path)

public: // Immediate helper methods
    virtual bool  checkDiskMounted(QString disk_path) = 0;
    virtual ImagerState getState() = 0;

public slots:
    // Boot Manipulation settings
    virtual void loadBootFromDevice(QString drive_path) = 0;  // Loads the bootfile into memory (finishedReadingBootSettings signal)
    virtual void overwriteBootFile(QByteArray& boot_data,  QString drive_path) = 0; // Overwrites the file on device with data passed

    // Direct Drive Methods
    virtual void mountDisk(QString disk_path) = 0;  // Attempt to mount the device located @
    virtual void unmountDisk(QString disk_path) = 0; // Attempt to unmount the device located @
    virtual void queryDevices(bool show_not_ready) = 0; // Search for some devices

    // Higher Level Flash Control Methods
    virtual void cancelCurrentOperation() = 0; // Quit what you are doing and clean up if possible, user was already warned!
    // better be mounted and ready to startImageWrite or startReadImage
    virtual void startImageWrite(QString root_path, QString image_path, bool should_verify, bool write_partitions)= 0;
    virtual void startReadImage(QString root_path, QString image_path, bool should_verify, bool read_partitions)= 0;

    // File system (cookie crumb) helpers
    virtual QVariantList temporaryCrumbs(QString temp_path) = 0;
    virtual QVariantList getCrumbPath(QString path) = 0;
    virtual QString getSystemRootPath() = 0;
    virtual QString getSystemRootName() = 0;
    virtual QString getSystemRootUrl() = 0;

protected:
    explicit DiskManager(QObject *parent = 0):QObject(parent){} // prevent creating virtual class
    ~DiskManager(){}
};

#endif // DISKMANAGER_H
