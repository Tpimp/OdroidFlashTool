#ifndef WINDOWSDISKMANAGER_H
#define WINDOWSDISKMANAGER_H
#ifndef WINVER
#define WINVER 0x0601
#endif





#include <QTimer>
#include "md5.h"
#include <QObject>

#include "diskmanager.h"



class WindowsDiskManager : public DiskManager
{
    Q_OBJECT
public:

    WindowsDiskManager(QObject *parent = 0);
    ~WindowsDiskManager();
    bool checkDiskMounted(QString disk_path){
        if(mMountedToDrive && (mMountedPath.compare(disk_path) == 0))
        {
            return true;
        }
        return false;
    }
    ImagerState getState()
    {
        return mCurrentState;
    }

signals:
/***************************************************************************************************
 * Signal inherited from DiskManager
    void  bootSettingsNotFound(int error, QString& error_str);
    void  errorOccured(ImagerState state,QString error);   // Important for anything...
    void  finishedReadingBootSettings(QByteArray& settings); // Got your boot settings...
    void  finishedWritingBootSettings(QString drive_path);   // Set your boot settings...
    void  foundDevice(QString device, QString device_path);  // in response to query...
    void  mountedDisk(QString disk_path);
    void  processCompleted(int percentage, QString speed);
    void  processFinished(ImagerState state);  // Finished some state - what next? - READY...
    void  processStarted(ImagerState state);   // Hey! Im going to start doing something now,...
    void  unmountedDisk(QString disk_path);    // All finished with the unmount process on....
****************************************************************************************************/
    // add Windows specific signals

public slots:
    void loadBootFromDevice(QString drive_path);  // Loads the bootfile into memory (finishedReadingBootSettings signal)
    void overwriteBootFile(QByteArray& boot_data,  QString drive_path); // Overwrites the file on device with data passed

    // Direct Drive Methods
    void mountDisk(QString disk_path);  // Attempt to mount the device located @
    void unmountDisk(QString disk_path); // Attempt to unmount the device located @
    void queryDevices(bool show_not_ready); // Search for some devices

    // Higher Level Flash Control Methods
    void cancelCurrentOperation(); // Quit what you are doing and clean up if possible, user was already warned!
    // better be mounted and ready to startImageWrite or startReadImage
    void startImageWrite(QString root_path, QString image_path, bool should_verify, bool write_partitions);
    void startReadImage(QString root_path, QString image_path, bool should_verify, bool read_partitions);


    // File system (cookie crumb) helpers
    QVariantList temporaryCrumbs(QString temp_path);
    QVariantList getCrumbPath(QString path);
    QString getSystemRootPath();
    QString getSystemRootName();
    QString getSystemRootUrl();

private:// windows specific members (windows handles,...)

    // Internal Functions to perform DiskManager methods on Windows
    ImagerState   mCurrentState = NOT_READY_STATE;
    bool          mMountedToDrive = false;
    QString mMountedPath;

private slots:
    void readingImageFinished(QString filename);
};

#endif // WINDOWSDISKMANAGER_H
