#ifndef WINDOWSDISKUTILITY_H
#define WINDOWSDISKUTILITY_H



#include <shlobj.h>
#include <iostream>
#include <sstream>
#include <windows.h>
#include <winioctl.h>
#include <QStorageInfo>
#include <QString>
#include <QMap>
#include <QList>
#include <QPair>
#include <QFileInfo>
#include <QDirIterator>
#include <cstdio>
#include <cstdlib>
#include <QDebug>

#include "partitionfactory.h"
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

inline bool getRawDiskHandle(HANDLE & disk_handle, int disk_number, QString &error_str,DWORD access = FILE_SHARE_READ)
{
    QString device_name("\\\\.\\");
    device_name.append("PhysicalDrive");
    device_name.append(QString::number(disk_number));

    disk_handle = CreateFile(device_name.toStdWString().c_str(), access, FILE_SHARE_READ | FILE_SHARE_WRITE  , NULL, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH, NULL);
    if (disk_handle == INVALID_HANDLE_VALUE)
    {
        wchar_t *errormessage=NULL;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPWSTR)&errormessage, 0, NULL);
        error_str = QString::fromUtf16((const ushort *)errormessage);
        LocalFree(errormessage);
        return false;
    }

    return true;
}

inline quint16 getDriveDetails(char drive_letter, HANDLE volume_handle, HANDLE &device_handle, DiskMap &disk_map, int &disk_number, quint64& drive_size, QString & error_str)
{
    // Quite extensive lookup function - should be called as little as possible, with the caller working to maintain disk_details externally

    // First get a list of the volume infos

    VOLUME_DISK_EXTENTS volumeDiskExtents;
    DWORD dwBytesReturned = 0;
    QList<PDISK_EXTENT>    disk_details;
    DiskDescriptorInfo     disk_info;
    BOOL bResult = DeviceIoControl(  volume_handle,
                                     IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS,
                                     NULL,
                                     0,
                                     &volumeDiskExtents,
                                     sizeof(volumeDiskExtents),
                                     &dwBytesReturned,
                                     NULL);
    if(!bResult)
    {
        wchar_t *errormessage=NULL;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPWSTR)&errormessage, 0, NULL);
        error_str = QString::fromUtf16((const ushort *)errormessage);
        LocalFree(errormessage);
        qDebug() << "Failed to fetch the volume information with handle: " << long(volume_handle);
        return -1;
    }

    bool isFirst(true);

    if(volumeDiskExtents.NumberOfDiskExtents == 0)
    {
        qDebug() << "Fetched volume information was empty with handle: " << long(volume_handle);
        return -1;
    }

    // find the disk number
    for (DWORD n = 0;  n < volumeDiskExtents.NumberOfDiskExtents;++n)
    {
        PDISK_EXTENT pDiskExtent =  &volumeDiskExtents.Extents[n];
        disk_details.append(pDiskExtent);
        if(isFirst)
        {
            disk_number = pDiskExtent->DiskNumber;
            isFirst = false;
        }
    }




    uint16_t bytes_per_sector(512);
    MBR_TABLE   ebr_table;
    // Get Partition table
    QString device_name("\\\\.\\PhysicalDrive");
    device_name.append(QString::number(disk_number));

    WriteChunk       MBR;
    MBR.first = 0;
    MBR.second = 1;
    WriteChunk      FAT;

    QFile  file(device_name);
    if(!file.open(QIODevice::ReadOnly))
        qDebug() << "Failed to open disk";
    char  mbr_data[512];
    quint64 bytes_read = file.read(mbr_data,512);
    ebr_table.setData(mbr_data,bytes_read);
    FAT.first = ebr_table.Record1.LBAAddress;
    FAT.second = ebr_table.Record1.PartitionSectors;
    file.close();

    WriteChunk  linux_chunk;
    linux_chunk.first = ebr_table.Record2.LBAAddress;
    linux_chunk.second = ebr_table.Record2.PartitionSectors;
   // free_space.first = (linux_chunk.first + linux_chunk.second);

    DWORD spc,bps,freeclusters,totalclusters;
    wchar_t drive_name[] = {drive_letter,':','\\','\0'};
    BOOL result = GetDiskFreeSpace(drive_name,&spc,&bps,&freeclusters,&totalclusters);
    if(result == 0)
    {
        qDebug() << "Failed to get DiskFreeSpace";
        wchar_t *errormessage=NULL;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPWSTR)&errormessage, 0, NULL);
        qDebug() << "Error Message : " << QString::fromUtf16((const ushort *)errormessage);
        LocalFree(errormessage);
    }
    else
    {
        drive_size = spc * bps * totalclusters;
        //free_space = spc * bps * freeclusters;
        bytes_per_sector = bps;
    }
    bool success = getRawDiskHandle(device_handle,disk_number,error_str);
    if(!success)
    {
        qDebug() << "Failed to get device handle" << error_str;
    }
    else{
        DISK_GEOMETRY_EX geometry;
        DWORD bytes_returned;
        bResult = DeviceIoControl(device_handle,IOCTL_DISK_GET_DRIVE_GEOMETRY_EX,NULL,0,&geometry,sizeof(DISK_GEOMETRY_EX),&bytes_returned,NULL);
        if(bResult != 0)
        {
            qDebug() << "Geometry: " << geometry.DiskSize.QuadPart;
            drive_size = geometry.DiskSize.QuadPart;
            bytes_per_sector = geometry.Geometry.BytesPerSector;
        }
    }

    disk_info.volumeLength = drive_size;
    disk_info.writeChunks.append(MBR);
    disk_info.writeChunks.append(FAT);
    disk_info.writeChunks.append(linux_chunk);

    WriteChunk       free_space_partition;
    free_space_partition.first = ( (linux_chunk.first +linux_chunk.second) * bytes_per_sector);
    free_space_partition.second = (drive_size - free_space_partition.first);
    disk_info.writeChunks.append(free_space_partition);

    disk_map.insert(drive_letter,disk_info);

    return bytes_per_sector;
}
inline bool getVolumeHandle(char drive_letter, QFile & volume_handle, QString &error_str, HANDLE &handle_to_set,QIODevice::OpenModeFlag mode = QIODevice::ReadOnly)
{

    char volumeName[8] = "";
    const char* volumeFormat = "\\\\.\\%c:";
    sprintf(volumeName, volumeFormat, drive_letter);
    volume_handle.setFileName(volumeName);
    if(!volume_handle.open(mode))
    {
        qDebug()<< "Failed to open volume with mode" << mode;
        return false;
    }
    handle_to_set = HANDLE(_get_osfhandle(volume_handle.handle()));
    if(handle_to_set == INVALID_HANDLE_VALUE)
    {
        wchar_t *errormessage=NULL;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPWSTR)&errormessage, 0, NULL);
        error_str = QString::fromUtf16((const ushort *)errormessage);
        LocalFree(errormessage);
        return false;
    }
    return true;
}
inline bool lockVolume(HANDLE volume_handle, QString &error_str)
{
    DWORD bytesreturned;
    BOOL bResult(DeviceIoControl(volume_handle, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &bytesreturned, NULL));
    if (!bResult)
    {
        wchar_t *errormessage=NULL;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPWSTR)&errormessage, 0, NULL);
        error_str = QString::fromUtf16((const ushort *)errormessage);
        LocalFree(errormessage);
    }
    return bool(bResult);

}

inline bool    unmountVolume(HANDLE volume_handle, char drive_letter, QString &error_str)
{
    DWORD junk;
    BOOL bResult;
    wchar_t drive_path[]= {'A',':','\0'};
    drive_path[0] = drive_letter;
    bResult = DeviceIoControl(volume_handle, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &junk, NULL);
    DefineDosDevice(DDD_RAW_TARGET_PATH |DDD_REMOVE_DEFINITION  |DDD_EXACT_MATCH_ON_REMOVE,drive_path,drive_path);
    if (!bResult)
    {
        wchar_t *errormessage=NULL;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPWSTR)&errormessage, 0, NULL);
        error_str = QString::fromUtf16((const ushort *)errormessage);
        LocalFree(errormessage);
    }
    return bool(bResult);
}

inline bool    removeLock(HANDLE volume_handle, QString & error_str)
{
    DWORD junk;
    BOOL bResult;
    bResult = DeviceIoControl(volume_handle, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &junk, NULL);
    if (!bResult)
    {
        wchar_t *errormessage=NULL;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPWSTR)&errormessage, 0, NULL);
        error_str = QString::fromUtf16((const ushort *)errormessage);
        LocalFree(errormessage);
    }
    return bool(bResult);
}



inline bool    getFileSize(QFile & file_handle, QStorageInfo & image_drive, quint64 &file_size, quint64 &sector_size, quint64 & num_sectors,  QString &error_str)
{
    file_size =  file_handle.size();
    quint64 spaceneeded(0);
    spaceneeded = (num_sectors +1)  *sector_size;
    if (image_drive.bytesAvailable() <= spaceneeded)
    {
        spaceneeded = 0;
        error_str = "Not enough room on the write disk to create image.";
        return false;
    }
    return true;
}
#endif // WINDOWSDISKUTILITY_H
