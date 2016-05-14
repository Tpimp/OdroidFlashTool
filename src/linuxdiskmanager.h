#ifndef LINUXDISKMANAGER_H
#define LINUXDISKMANAGER_H

#include <QObject>

class LinuxDiskManager : public QObject
{
    Q_OBJECT
public:
    explicit LinuxDiskManager(QObject *parent = 0);

signals:

public slots:
    void  queryDiskLocations();
    void  queryBootIni();
    void  requestWriteImage(QString image_path, QString write_path);
    void  requestReadImage(QString read_path, QString image_path);

private: // linux specific members (inode handles,...)
};

#endif // LINUXDISKMANAGER_H
