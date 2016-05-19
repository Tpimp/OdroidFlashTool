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

#include "disk.h"
#include "md5.h"
class WindowsDiskManager : public QObject
{
    Q_OBJECT
public:
    explicit WindowsDiskManager(QObject *parent = 0);
    ~WindowsDiskManager();
signals:

public slots:
    void  queryDiskLocations();
    void  queryBootIni();
    void  requestWriteImage(QString image_path, QString write_path);
    void  requestReadImage(QString read_path, QString image_path);
    void  verifyDiskImage();
private:// windows specific members (windows handles,...)
    HANDLE mVolumeHandle;
    HANDLE mFileHandle;
    HANDLE mRawDiskHandle;
    unsigned long long sectorsize;
    int status;
    char *sectorData;
};

#endif // WINDOWSDISKMANAGER_H
