#ifndef ODROIDFLASHMANAGER_H
#define ODROIDFLASHMANAGER_H

#include <QObject>
#include "diskimager.h"
#include <QList>
#include "decompressionthread.h"
#include "compressionthread.h"
class OdroidFlashManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString mDevicePath READ device  WRITE setDevice NOTIFY deviceChanged)
    Q_PROPERTY(QString mTemporaryPath READ temporaryPath WRITE setTemporaryPath NOTIFY temporaryPathChanged)
public:
    explicit OdroidFlashManager(DiskImager * dskimg, QString application_dir, QObject *parent = 0);
    QString temporaryPath()
    {
        return mTemporaryPath;
    }

    QString device()
    {
        return mDevicePath;
    }
    enum ProcessID{DECOMPRESSING = 0,COMPRESSING, WRITING_IMAGE, READING_IMAGE, WRITING_GUID, MOUNTING, DOWNLOADING};
    Q_ENUM(ProcessID)

signals:
    void compressionFailed(QString error);
    void compressionCompleted(int percentage);
    void deviceChanged(QString device);
    void deviceInvalid(QString device);
    void decompressionComplete(int percentage);
    void errorHappened(OdroidFlashManager::ProcessID id, int error_code, QString error_string);
    void foundMountedDevice(QString device,QString root_path);
    void processStarted(OdroidFlashManager::ProcessID id);
    void processFinished(OdroidFlashManager::ProcessID id);
    void temporaryPathChanged(QString path);

public slots:
    void cancelCurrentOperation();
    void startDecompression(QString archive_path);
    void startCompression(QString image_path);
    void startImageWrite(QString image_path);
    void startReadImage(QString root_path);
    void queryMountedDevices();
    void setTemporaryPath(QString path)
    {
        if(path.compare(mTemporaryPath) != 0)
        {
            mTemporaryPath = path;
            emit temporaryPathChanged(mTemporaryPath);
        }
    }

    void setDevice(QString device_path)
    {
        if(device_path.compare(mDevicePath))
        {
            mDevicePath = device_path;
            emit deviceChanged(mDevicePath);
        }
    }


private:
    DiskImager *     mDiskImager;
    QString          mDevicePath;
    QString          mTemporaryPath;
    QString          mApplicationDir;
    QList<ProcessID> mRunningProcesses;
    DecompressionThread * mDecompressThread = nullptr;
    CompressionThread   * mCompressionThread = nullptr;

private slots:
    void  compressionFinished();
    void  decompressionFinished();


};

#endif // ODROIDFLASHMANAGER_H
