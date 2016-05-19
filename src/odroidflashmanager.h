#ifndef ODROIDFLASHMANAGER_H
#define ODROIDFLASHMANAGER_H

#include <QObject>
#include "diskimager.h"
#include <QList>
class OdroidFlashManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString mDevicePath READ device  WRITE setDevice NOTIFY deviceChanged)
public:
    explicit OdroidFlashManager(DiskImager * dskimg, QObject *parent = 0);
    QString device()
    {
        return mDevicePath;
    }
    enum ProcessID{DECOMPRESSING = 0,COMPRESSING, WRITING_IMAGE, READING_IMAGE, WRITING_GUID, MOUNTING, DOWNLOADING};
    Q_ENUM(ProcessID)

signals:
    void deviceChanged(QString device);
    void deviceInvalid(QString device);
    void decompressionComplete(int percentage);
    void decompressionFailed(QString error);
    void errorHappened(ProcessID id, int error_code, QString error_string);

public slots:
    void startDecompression(QString archive_path);
    void cancelCurrentOperation();
    void setDevice(QString device_path)
    {
        if(device_path.compare(mDevicePath))
        {
            mDevicePath = device_path;
            emit deviceChanged(mDevicePath);
        }
    }
    void startImageWrite(QString image_path);

private:
    DiskImager *     mDiskImager;
    QString          mDevicePath;
    QList<ProcessID> mRunningProcesses;
};

#endif // ODROIDFLASHMANAGER_H
