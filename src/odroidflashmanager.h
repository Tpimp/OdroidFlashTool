#ifndef ODROIDFLASHMANAGER_H
#define ODROIDFLASHMANAGER_H

#include <QObject>
#include "diskimager.h"
#include <QList>
#include "decompressionthread.h"
#include "compressionthread.h"
#include <QUrl>
#ifdef Q_OS_WIN
    #include "windowsdiskreaderthread.h"
    #include "windowsdiskwriterthread.h"
#else

#endif

class ApplicationSettings;

class OdroidFlashManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString mDevicePath READ device  WRITE setDevice NOTIFY deviceChanged)
public:
    explicit OdroidFlashManager(DiskImager * dskimg,ApplicationSettings* settings, QString application_dir, QObject *parent = 0);
    Q_INVOKABLE QUrl temporaryUrl();
    Q_INVOKABLE QVariantList temporaryCrumbs();
    Q_INVOKABLE QVariantList getCrumbPath(QString path);

    Q_INVOKABLE QString getSystemRootPath();
    Q_INVOKABLE QString getSystemRootName();
    Q_INVOKABLE QString getSystemRootUrl();
    Q_INVOKABLE void    updateProcessView();

    QString device()
    {
        return mDevicePath;
    }

    enum ProcessID{DECOMPRESSING = 0,COMPRESSING, WRITING_IMAGE, WRITING_IMAGE_VERIFY, READING_IMAGE, READING_IMAGE_VERIFY,WRITING_BOOT, READING_BOOT, RESTORING_BOOT, DELETE_IMAGE, WRITING_GUID, MOUNTING, DOWNLOADING, NULL_PROCESS};
    enum FlashMode{READ = 0, WRITE = 1};
    Q_ENUM(ProcessID)
    Q_ENUM(FlashMode)

signals:
    void appendProcess(QString name);
    void backedUpBootIni();
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
    void backupBootIni(QString boot_ini_path);
    void writeBootIni(QString boot_ini_path, QString drive_path);
    void cancelCurrentOperations();
    void restoreLastBootBackup(QString drive_path);
    void startProcedure();
    void startDecompression(QString archive_path);
    void startCompression(QString image_path);
    void startImageWrite(QString root_path, QString image_path, bool should_verify, bool write_partitions);
    void startReadImage(QString root_path, QString image_path, bool should_verify, bool read_partitions);
    void startNextProcess();
    void queryMountedDevices();


    void setDevice(QString device_path)
    {
        if(device_path.compare(mDevicePath))
        {
            mDevicePath = device_path;
            emit deviceChanged(mDevicePath);
        }
    }


private:
    DiskImager*           mDiskImager;
    ApplicationSettings*  mAppSettings;
    QString               mFavoritesPath;
    QString               mDevicePath;
    QString               mApplicationDir;
    QString               mCurrentSource;
    QString               mCurrentTarget;
    ProcessID             mCurrentProcess = NULL_PROCESS;
    QList<ProcessID>      mRunningProcesses;

    // Threaded tasks
    DecompressionThread*  mDecompressThread = nullptr;
    CompressionThread*    mCompressionThread = nullptr;
    ReaderThread*         mReaderThread = nullptr;
    WriterThread*         mWriterThread = nullptr;

private slots:
    void  compressionFinished(QString file_out);
    void  decompressionFinished(QString file_out);
    void  readingImageFinished(QString filename);
    void  writingImageFinished();
    void  processReadErrors(ReaderThread::ReaderError error, QString error_str);
    void  processWriteErrors(WriterThread::WriteError error, QString error_str);

};

#endif // ODROIDFLASHMANAGER_H
