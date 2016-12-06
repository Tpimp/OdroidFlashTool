#ifndef ODROIDFLASHMANAGER_H
#define ODROIDFLASHMANAGER_H

#include <QObject>
#include <QList>
#include "decompressionthread.h"
#include "compressionthread.h"
#include <QUrl>
#include "bootmanager.h"
#include "compressionmanager.h"
#include "diskmanager.h"
#ifdef Q_OS_WIN
//    #include "windowsdiskreaderthread.h"
//    #include "windowsdiskwriterthread.h"
#else

#endif

class ApplicationSettings;

class OdroidFlashManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString mDevicePath READ device  WRITE setDevice NOTIFY deviceChanged)
public:
    explicit OdroidFlashManager(DiskManager * disk_manager,ApplicationSettings* settings, QString application_dir, QObject *parent = 0);
    Q_INVOKABLE QUrl temporaryUrl();
    Q_INVOKABLE QVariantList temporaryCrumbs();
    Q_INVOKABLE QVariantList getCrumbPath(QString path);
    // TODO: convert these to using DiskManager
    Q_INVOKABLE QString getSystemRootPath();
    Q_INVOKABLE QString getSystemRootName();
    Q_INVOKABLE QString getSystemRootUrl();
    Q_INVOKABLE void    updateProcessView();

    QString device()
    {
        return mDevicePath;
    }

    enum ProcessID{DECOMPRESSING = 0,COMPRESSING, WRITING_IMAGE, WRITING_IMAGE_VERIFY, READING_IMAGE, READING_IMAGE_VERIFY,WRITING_BOOT, READING_BOOT, RESTORING_BOOT, DELETE_IMAGE, WRITING_GUID, MOUNTING, DOWNLOADING,WAITING_PROCESS, NULL_PROCESS};
    enum FlashMode{READ = 0, WRITE = 1};

signals:
    void appendProcess(QString name);
    void backedUpBootIni();
    void bootFileLoaded(QString file_name);
    void bootFileLoadedDevice(QString device);
    void operationCanceled(QString operation);
    void compressionFailed(QString error);
    void compressionCompleted(int percentage);
    void clearProcessList();
    void deviceChanged(QString device);
    void deviceInvalid(QString device);
    void decompressionComplete(int percentage);
    void errorHappened(OdroidFlashManager::ProcessID id, int error_code, QString error_string);
    void foundMountedDevice(QString device,QString root_path);
    void insertProcess(int index, QString name);
    void processStarted(OdroidFlashManager::ProcessID id);
    void processFinished(OdroidFlashManager::ProcessID id, QString data = "");
    void shouldCompressChanged(bool should_compress);
    void startedReadPartitions(QString volume, int current_volume, int total_volumes);
    void startedWritePartitions(QString volume, int current_volume, int total_volumes);
    void readCompleted(int percentage, QString read_speed, QString time_left);
    void removeProcessAt(int index);
    void writeCompleted(int percentage, QString write_speed, QString time_left);
    void wroteBootIni();
    void temporaryPathChanged(QString path);

public slots:
    void writeMemoryToBootFile(QString disk_path,QString device_format);
    void loadBootFromFile(QString boot_file_path);
    void cancelCurrentOperations();
    void restoreBackupFile(QString disk_path);
    void startProcedure();
    void startNextProcess();

    //Disk Wrapper Methods
    void requestBackupBootFile(QString boot_file_path, QString disk_path);
    void loadBootFromDevice(QString disk_path);
    void queryMountedDevices();
    void startImageWrite(QString root_path, QString image_path, bool should_verify, bool write_partitions);
    void startReadImage(QString root_path, QString image_path, bool should_verify, bool read_partitions);
    void writeBootFromFile(QString boot_ini_path,  QString drive_path);

    // Compression Wrapper Methods
    void startDecompression(QString archive_path);
    void startCompression(QString image_path);

    void setDevice(QString device_path)
    {
        if(device_path.compare(mDevicePath))
        {
            mDevicePath = device_path;
            emit deviceChanged(mDevicePath);
        }
    }


private:
    //DiskImager*           mDiskImager;
    ApplicationSettings*  mAppSettings;
    QString               mFavoritesPath;
    QString               mDevicePath;
    QString               mApplicationDir;
    QString               mCurrentSource;
    QString               mCurrentTarget;
    ProcessID             mCurrentProcess = NULL_PROCESS;
    QList<ProcessID>      mRunningProcesses;

    // Threaded tasks
    BootManager*          mBootManager;  // manages local boot files, and device boot formats
    DiskManager*          mDiskManager;
    CompressionManager*   mCompressionManager;

    //DecompressionThread*  mDecompressThread = nullptr;
    //CompressionThread*    mCompressionThread = nullptr;
    //ReaderThread*         mReaderThread = nullptr;
    //WriterThread*         mWriterThread = nullptr;

private slots:

    // DiskManager Signals
    void processBootSettingRecieved(QByteArray & settings);
    void processBootSettingsWritten(QString disk_path);
    void processBootSettingsNotFound(int error, QString& error_str);
    void processDiskError(DiskManager::ImagerState state,QString error);
    void processDiskFinished(DiskManager::ImagerState state);
    void processDiskMounted(QString disk_path);
    void processDiskStarted(DiskManager::ImagerState state);
    void processDiskUnmounted(QString disk_path);
    void processFoundDevice(QString device, QString device_path);

};

#endif // ODROIDFLASHMANAGER_H
