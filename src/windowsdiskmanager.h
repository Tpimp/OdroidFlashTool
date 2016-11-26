#ifndef WINDOWSDISKMANAGER_H
#define WINDOWSDISKMANAGER_H
#ifndef WINVER
#define WINVER 0x0601
#endif


#include <QFileInfo>
#include <QDirIterator>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <winioctl.h>
#include <dbt.h>
#include <shlobj.h>
#include <iostream>
#include <sstream>
#include <QTimer>
#include "md5.h"
#include <QStorageInfo>
#include <QString>
#include <QMap>
#include <QList>
#include <QPair>
typedef QPair<quint64,quint64> WriteChunk;
typedef QList<WriteChunk> WriteList;

typedef struct DiskDescriptorInfo
{
    quint64    volumeLength;
    QString    volumeGUID;
    WriteList  writeChunks;
    quint64    bytesAvailable;
    quint64    bytesFree;
    quint64    totalBytes;
}Disk_Descriptor_Info, *PDisk_Descriptor_Info;

typedef QMap<char, DiskDescriptorInfo> DiskMap;

//TODO: Add a static method for dismounting the drive, and a method to remount a drive

class WindowsDiskManager
{
public:

    static quint16 getDriveDetails(char drive_letter, HANDLE volume_handle, HANDLE &device_handle, DiskMap &disk_map, int &disk_number, quint64& drive_size, QString & error_str);
    static bool    getVolumeHandle(char drive_letter, QFile & volume_handle, QString &error_str, HANDLE &handle_to_set,QIODevice::OpenModeFlag mode = QIODevice::ReadOnly);
    static bool    lockVolume(HANDLE volume_handle, QString &error_str);
    static bool    unmountVolume(HANDLE volume_handle, char drive_letter, QString &error_str);
    static bool    removeLock(HANDLE volume_handle, QString & error_str);
    static bool    getRawDiskHandle(HANDLE & disk_handle, int disk_number, QString &error_str,DWORD access = FILE_SHARE_READ);
    static bool    getFileSize(QFile & file_handle, QStorageInfo & image_drive, quint64 &file_size, quint64 &sector_size, quint64 & num_sectors,  QString &error_str);

private:// windows specific members (windows handles,...)

    explicit WindowsDiskManager(){}
    ~WindowsDiskManager(){}
};

#endif // WINDOWSDISKMANAGER_H
