#ifndef ODROIDFLASHMANAGER_H
#define ODROIDFLASHMANAGER_H

#include <QObject>
#include "diskimager.h"
class OdroidFlashManager : public QObject
{
    Q_OBJECT
public:
    explicit OdroidFlashManager(DiskImager * dskimg, QObject *parent = 0);

signals:

public slots:

private:
    DiskImager *   mDiskImager;
};

#endif // ODROIDFLASHMANAGER_H
