#include "odroidflashmanager.h"

OdroidFlashManager::OdroidFlashManager(DiskImager *dskimg, QObject *parent) : QObject(parent), mDiskImager(dskimg)
{

}
