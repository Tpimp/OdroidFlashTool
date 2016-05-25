#include "windowsdiskmanager.h"
#include <QStorageInfo>
#include <QDebug>

WindowsDiskManager::WindowsDiskManager(QObject *parent) : QObject(parent)
{
    mVolumeHandle = INVALID_HANDLE_VALUE;
    mFileHandle = INVALID_HANDLE_VALUE;
    mRawDiskHandle = INVALID_HANDLE_VALUE;

    sectorData = NULL;
    sectorsize = 0ul;

}

void WindowsDiskManager::queryBootIni()
{

}


void WindowsDiskManager::requestReadImage(QString read_path, QString image_path)
{

    QStorageInfo  read_drive(read_path);
    QStorageInfo  image_drive(image_path);
    int deviceID(0);
    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
            if (storage.isValid() && storage.isReady() && !storage.isRoot()) {
                if(storage.rootPath().compare("C://") != 0)
                {
                    deviceID++;
                    if(storage.rootPath().compare(read_drive.rootPath()) == 0)
                        break;
                }
            }
        }



    if(read_drive.rootPath().compare(image_drive.rootPath()) == 0)
    {
        emit error(tr("Write Error") + " : " +tr("Image file cannot be located on the target device."));
    }
    double mbpersec;
    unsigned long long i, lasti, numsectors, filesize, spaceneeded = 0ull;
    qDebug() << "Device " << read_drive.device();
    int volumeID = read_path.at(0).toLatin1() - 'A'; // convert letter to a a value
    mVolumeHandle = getHandleOnVolume(volumeID, GENERIC_READ);
    if (mVolumeHandle == INVALID_HANDLE_VALUE)
    {
        error(tr("Write Error") + " : Failed to fetch volume handle " + read_path);
        return;
    }
    if (!getLockOnVolume(mVolumeHandle))
    {
        CloseHandle(mVolumeHandle);
        mVolumeHandle = INVALID_HANDLE_VALUE;
        error(tr("Write Error") + " : Failed to lock volume " + read_path);
        return;
    }
    if (!unmountVolume(mVolumeHandle))
    {
        removeLockOnVolume(mVolumeHandle);
        CloseHandle(mVolumeHandle);
        mVolumeHandle = INVALID_HANDLE_VALUE;
        error(tr("Write Error") + " : Failed to lock volume " + read_path);
        return;
    }
    mFileHandle = getHandleOnFile(LPCWSTR(image_path.data()), GENERIC_WRITE);
    if (mFileHandle == INVALID_HANDLE_VALUE)
    {
        removeLockOnVolume(mVolumeHandle);
        CloseHandle(mVolumeHandle);
        mVolumeHandle = INVALID_HANDLE_VALUE;
        error(tr("Write Error") + " : Failed to lock volume " + read_path);
        return;
    }
    mRawDiskHandle = getHandleOnDevice(deviceID, GENERIC_READ);
    if (mRawDiskHandle == INVALID_HANDLE_VALUE)
    {
        removeLockOnVolume(mVolumeHandle);
        CloseHandle(mFileHandle);
        CloseHandle(mVolumeHandle);
        mVolumeHandle = INVALID_HANDLE_VALUE;
        mFileHandle = INVALID_HANDLE_VALUE;
        error(tr("Write Error") + " : Failed to lock volume " + read_path);
        return;
    }
    numsectors = getNumberOfSectors(mRawDiskHandle, &sectorsize);
    if(mReadOnlyPartitions)
    {
        // Read MBR partition table
        sectorData = readSectorDataFromHandle(mRawDiskHandle, 0, 1ul, 512ul);
        numsectors = 1ul;
        // Read partition information
        for (i=0ul; i<4ul; i++)
        {
            uint32_t partitionStartSector = *((uint32_t*) (sectorData + 0x1BE + 8 + 16*i));
            uint32_t partitionNumSectors = *((uint32_t*) (sectorData + 0x1BE + 12 + 16*i));
            // Set numsectors to end of last partition
            if (partitionStartSector + partitionNumSectors > numsectors)
            {
                numsectors = partitionStartSector + partitionNumSectors;
            }
        }
    }
    filesize = getFileSizeInSectors(mFileHandle, sectorsize);
    if (filesize >= numsectors)
    {
        spaceneeded = 0ull;
    }
    else
    {
        spaceneeded = (unsigned long long)(numsectors - filesize) * (unsigned long long)(sectorsize);
    }
    if (image_drive.bytesAvailable() <= spaceneeded)
    {
        removeLockOnVolume(mVolumeHandle);
        CloseHandle(mRawDiskHandle);
        CloseHandle(mFileHandle);
        CloseHandle(mVolumeHandle);
        sectorData = NULL;
        mRawDiskHandle = INVALID_HANDLE_VALUE;
        mFileHandle = INVALID_HANDLE_VALUE;
        mVolumeHandle = INVALID_HANDLE_VALUE;
        emit error(tr("Write Error") + tr(" : Disk is not large enough for the specified image."));
        return;
    }
    // figure out how to alter UI based on num sectors
//    if (numsectors == 0ul)
//    {
//        progressbar->setRange(0, 100);
//    }
//    else
//    {
//        progressbar->setRange(0, (int)numsectors);
//    }
    lasti = 0ul;


    // Do the actual work (Read the image)
    bool isReading(true);
    for (i = 0ul; i < numsectors && isReading; i += 1024ul)
    {
        sectorData = readSectorDataFromHandle(mRawDiskHandle, i, (numsectors - i >= 1024ul) ? 1024ul:(numsectors - i), sectorsize);
        if (sectorData == NULL)
        {
            removeLockOnVolume(mVolumeHandle);
            CloseHandle(mRawDiskHandle);
            CloseHandle(mFileHandle);
            CloseHandle(mVolumeHandle);
            mRawDiskHandle = INVALID_HANDLE_VALUE;
            mFileHandle = INVALID_HANDLE_VALUE;
            mVolumeHandle = INVALID_HANDLE_VALUE;
            emit error(tr("Write Error") + " : REACHED A NULL SECTOR");
            return;
        }
        if (!writeSectorDataToHandle(mFileHandle, sectorData, i, (numsectors - i >= 1024ul) ? 1024ul:(numsectors - i), sectorsize))
        {
            delete[] sectorData;
            removeLockOnVolume(mVolumeHandle);
            CloseHandle(mRawDiskHandle);
            CloseHandle(mFileHandle);
            CloseHandle(mVolumeHandle);
            sectorData = NULL;
            mRawDiskHandle = INVALID_HANDLE_VALUE;
            mFileHandle = INVALID_HANDLE_VALUE;
            mVolumeHandle = INVALID_HANDLE_VALUE;
            emit error(tr("Write Error") + " : Failed to write sector to file!");
            return;
        }
        delete[] sectorData;
        sectorData = NULL;
//        if ()
//        {
//            mbpersec = (((double)sectorsize * (i - lasti)) * ((float)ONE_SEC_IN_MS / update_timer.elapsed())) / 1024.0 / 1024.0;
//            statusbar->showMessage();
//            update_timer.start();
//            elapsed_timer->update(i, numsectors);
//            lasti = i;
//            emit writeCompleted((sectorsize * i)/(sectorsize *numsectors),QString("%1MB/s").arg(mbpersec));
//        }
    }
    removeLockOnVolume(mVolumeHandle);
    CloseHandle(mRawDiskHandle);
    CloseHandle(mFileHandle);
    CloseHandle(mVolumeHandle);
    mRawDiskHandle = INVALID_HANDLE_VALUE;
    mFileHandle = INVALID_HANDLE_VALUE;
    mVolumeHandle = INVALID_HANDLE_VALUE;

}


void WindowsDiskManager::requestWriteImage(QString image_path, QString write_path)
{
    // TODO: Convert old widget based "write" logic. This code ran when "Write Image" was clicked.
    /*
    bool passfail = true;
    if (!leFile->text().isEmpty())
    {
        QFileInfo fileinfo(leFile->text());
        if (fileinfo.exists() && fileinfo.isFile() &&
                fileinfo.isReadable() && (fileinfo.size() > 0) )
        {
            if (leFile->text().at(0) == cboxDevice->currentText().at(1))
            {
                QMessageBox::critical(NULL, tr("Write Error"), tr("Image file cannot be located on the target device."));
                return;
            }

            // build the drive letter as a const char *
            //   (without the surrounding brackets)
            QString qs = cboxDevice->currentText();
            qs.replace(QRegExp("[\\[\\]]"), "");
            QByteArray qba = qs.toLocal8Bit();
            const char *ltr = qba.data();
            if (QMessageBox::warning(NULL, tr("Confirm overwrite"), tr("Writing to a physical device can corrupt the device.\n"
                                                                       "(Target Device: %1 \"%2\")\n"
                                                                       "Are you sure you want to continue?").arg(cboxDevice->currentText()).arg(getDriveLabel(ltr)),
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
            {
                return;
            }
            status = STATUS_WRITING;
            bCancel->setEnabled(true);
            bWrite->setEnabled(false);
            bRead->setEnabled(false);
            bVerify->setEnabled(false);
            double mbpersec;

            #ifndef DONT_VERIFY_WRITE
                        int compare_result;
            #endif

            unsigned long long i, lasti, availablesectors, numsectors;
            int volumeID = cboxDevice->currentText().at(1).toLatin1() - 'A';
            int deviceID = cboxDevice->itemData(cboxDevice->currentIndex()).toInt();
            hVolume = getHandleOnVolume(volumeID, GENERIC_WRITE);
            if (hVolume == INVALID_HANDLE_VALUE)
            {
                status = STATUS_IDLE;
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            if (!getLockOnVolume(hVolume))
            {
                CloseHandle(hVolume);
                status = STATUS_IDLE;
                hVolume = INVALID_HANDLE_VALUE;
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            if (!unmountVolume(hVolume))
            {
                removeLockOnVolume(hVolume);
                CloseHandle(hVolume);
                status = STATUS_IDLE;
                hVolume = INVALID_HANDLE_VALUE;
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            hFile = getHandleOnFile(LPCWSTR(leFile->text().data()), GENERIC_READ);
            if (hFile == INVALID_HANDLE_VALUE)
            {
                removeLockOnVolume(hVolume);
                CloseHandle(hVolume);
                status = STATUS_IDLE;
                hVolume = INVALID_HANDLE_VALUE;
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            //hRawDisk = getHandleOnDevice(deviceID, GENERIC_WRITE);
            hRawDisk = getHandleOnDevice(deviceID, GENERIC_WRITE|GENERIC_READ);
            if (hRawDisk == INVALID_HANDLE_VALUE)
            {
                removeLockOnVolume(hVolume);
                CloseHandle(hFile);
                CloseHandle(hVolume);
                status = STATUS_IDLE;
                hVolume = INVALID_HANDLE_VALUE;
                hFile = INVALID_HANDLE_VALUE;
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            availablesectors = getNumberOfSectors(hRawDisk, &sectorsize);
            numsectors = getFileSizeInSectors(hFile, sectorsize);
            if (numsectors > availablesectors)
            {
                bool datafound = false;
                i = availablesectors;
                unsigned long nextchunksize = 0;
                while ( (i < numsectors) && (datafound == false) )
                {
                    nextchunksize = ((numsectors - i) >= 1024ul) ? 1024ul : (numsectors - i);
                    sectorData = readSectorDataFromHandle(hFile, i, nextchunksize, sectorsize);
                    if(sectorData == NULL)
                    {
                        // if there's an error verifying the truncated data, just move on to the
                        //  write, as we don't care about an error in a section that we're not writing...
                        i = numsectors + 1;
                    } else {
                        unsigned int j = 0;
                        unsigned limit = nextchunksize * sectorsize;
                        while ( (datafound == false) && ( j < limit ) )
                        {
                            if(sectorData[j++] != 0)
                            {
                                datafound = true;
                            }
                        }
                        i += nextchunksize;
                    }
                }
                // delete the allocated sectorData
                delete[] sectorData;
                sectorData = NULL;
                // build the string for the warning dialog
                std::ostringstream msg;
                msg << "More space required than is available:"
                    << "\n  Required: " << numsectors << " sectors"
                    << "\n  Available: " << availablesectors << " sectors"
                    << "\n  Sector Size: " << sectorsize
                    << "\n\nThe extra space " << ((datafound) ? "DOES" : "does not") << " appear to contain data"
                    << "\n\nContinue Anyway?";
                if(QMessageBox::warning(NULL, tr("Not enough available space!"),
                                        tr(msg.str().c_str()), QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok)
                {
                    // truncate the image at the device size...
                    numsectors = availablesectors;
                }
                else    // Cancel
                {
                    removeLockOnVolume(hVolume);
                    CloseHandle(hRawDisk);
                    CloseHandle(hFile);
                    CloseHandle(hVolume);
                    status = STATUS_IDLE;
                    hVolume = INVALID_HANDLE_VALUE;
                    hFile = INVALID_HANDLE_VALUE;
                    hRawDisk = INVALID_HANDLE_VALUE;
                    bCancel->setEnabled(false);
                    setReadWriteButtonState();
                    return;
                }
            }

            progressbar->setRange(0, (numsectors == 0ul) ? 100 : (int)numsectors);
            lasti = 0ul;
            update_timer.start();
            elapsed_timer->start();
            for (i = 0ul; i < numsectors && status == STATUS_WRITING; i += 1024ul)
            {
                sectorData = readSectorDataFromHandle(hFile, i, (numsectors - i >= 1024ul) ? 1024ul:(numsectors - i), sectorsize);
                if (sectorData == NULL)
                {
                    removeLockOnVolume(hVolume);
                    CloseHandle(hRawDisk);
                    CloseHandle(hFile);
                    CloseHandle(hVolume);
                    status = STATUS_IDLE;
                    hRawDisk = INVALID_HANDLE_VALUE;
                    hFile = INVALID_HANDLE_VALUE;
                    hVolume = INVALID_HANDLE_VALUE;
                    bCancel->setEnabled(false);
                    setReadWriteButtonState();
                    return;
                }
                if (!writeSectorDataToHandle(hRawDisk, sectorData, i, (numsectors - i >= 1024ul) ? 1024ul:(numsectors - i), sectorsize))
                {
                    delete[] sectorData;
                    removeLockOnVolume(hVolume);
                    CloseHandle(hRawDisk);
                    CloseHandle(hFile);
                    CloseHandle(hVolume);
                    status = STATUS_IDLE;
                    sectorData = NULL;
                    hRawDisk = INVALID_HANDLE_VALUE;
                    hFile = INVALID_HANDLE_VALUE;
                    hVolume = INVALID_HANDLE_VALUE;
                    bCancel->setEnabled(false);
                    setReadWriteButtonState();
                    return;
                }
#ifndef DONT_VERIFY_WRITE
                sectorData2 = readSectorDataFromHandle(hRawDisk, i, (numsectors - i >= 1024ul) ? 1024ul:(numsectors - i), sectorsize);
                if (sectorData2 == NULL)
                {
                    delete sectorData2;
                    removeLockOnVolume(hVolume);
                    CloseHandle(hRawDisk);
                    CloseHandle(hFile);
                    CloseHandle(hVolume);
                    status = STATUS_IDLE;
                    sectorData2 = NULL;
                    hRawDisk = INVALID_HANDLE_VALUE;
                    hFile = INVALID_HANDLE_VALUE;
                    hVolume = INVALID_HANDLE_VALUE;
                    bCancel->setEnabled(false);
                    setReadWriteButtonState();
                    return;
                }

                compare_result = memcmp( sectorData, sectorData2, ((numsectors - i >= 1024ul) ? 1024ul:(numsectors - i) * sectorsize) );
                if (compare_result != 0)
                {
                    delete[] sectorData;
                    delete[] sectorData2;
                    sectorData  = NULL;
                    sectorData2 = NULL;
                    removeLockOnVolume(hVolume);
                    CloseHandle(hRawDisk);
                    CloseHandle(hFile);
                    CloseHandle(hVolume);
                    status = STATUS_IDLE;
                    sectorData2 = NULL;
                    hRawDisk = INVALID_HANDLE_VALUE;
                    hFile = INVALID_HANDLE_VALUE;
                    hVolume = INVALID_HANDLE_VALUE;
                    bCancel->setEnabled(false);
                    setReadWriteButtonState();
                    QMessageBox::critical(NULL, tr("Verification Error"), tr("Read back data is different from original data."));
                    return;
                }
#endif
                delete[] sectorData;
                sectorData = NULL;
#ifndef DONT_VERIFY_WRITE
                delete[] sectorData2;
                sectorData2 = NULL;
#endif
                QCoreApplication::processEvents();
                if (update_timer.elapsed() >= ONE_SEC_IN_MS)
                {
                    mbpersec = (((double)sectorsize * (i - lasti)) * ((float)ONE_SEC_IN_MS / update_timer.elapsed())) / 1024.0 / 1024.0;
                    statusbar->showMessage(QString("%1MB/s").arg(mbpersec));
                    elapsed_timer->update(i, numsectors);
                    update_timer.start();
                    lasti = i;
                }
                progressbar->setValue(i);
                QCoreApplication::processEvents();
            }
            removeLockOnVolume(hVolume);
            CloseHandle(hRawDisk);
            CloseHandle(hFile);
            CloseHandle(hVolume);
            hRawDisk = INVALID_HANDLE_VALUE;
            hFile = INVALID_HANDLE_VALUE;
            hVolume = INVALID_HANDLE_VALUE;
            if (status == STATUS_CANCELED){
                passfail = false;
            }
        }
        else if (!fileinfo.exists() || !fileinfo.isFile())
        {
            QMessageBox::critical(NULL, tr("File Error"), tr("The selected file does not exist."));
            passfail = false;
        }
        else if (!fileinfo.isReadable())
        {
            QMessageBox::critical(NULL, tr("File Error"), tr("You do not have permision to read the selected file."));
            passfail = false;
        }
        else if (fileinfo.size() == 0)
        {
            QMessageBox::critical(NULL, tr("File Error"), tr("The specified file contains no data."));
            passfail = false;
        }
        progressbar->reset();
        statusbar->showMessage(tr("Done."));
        bCancel->setEnabled(false);
        setReadWriteButtonState();
        if (passfail){
            QMessageBox::information(NULL, tr("Complete"), tr("Write Successful."));
        }

    }
    else
    {
        QMessageBox::critical(NULL, tr("File Error"), tr("Please specify an image file to use."));
    }
    if (status == STATUS_EXIT)
    {
        close();
    }
    status = STATUS_IDLE;
    elapsed_timer->stop();
    */
}

void WindowsDiskManager::verifyDiskImage()
{
    // TODO: Convert old widget based "verify" logic. This code ran when "Verify Image" was clicked.
    /*bool passfail = true;
    if (!leFile->text().isEmpty())
    {
        QFileInfo fileinfo(leFile->text());
        if (fileinfo.exists() && fileinfo.isFile() &&
                fileinfo.isReadable() && (fileinfo.size() > 0) )
        {
            if (leFile->text().at(0) == cboxDevice->currentText().at(1))
            {
                QMessageBox::critical(NULL, tr("Write Error"), tr("Image file cannot be located on the target device."));
                return;
            }

            // build the drive letter as a const char *
            //   (without the surrounding brackets)
            QString qs = cboxDevice->currentText();
            qs.replace(QRegExp("[\\[\\]]"), "");
            QByteArray qba = qs.toLocal8Bit();
#if 0
            const char *ltr = qba.data();
            if (QMessageBox::warning(NULL, tr("Confirm overwrite"), tr("Writing to a physical device can corrupt the device.\n"
                                                                       "(Target Device: %1 \"%2\")\n"
                                                                       "Are you sure you want to continue?").arg(cboxDevice->currentText()).arg(getDriveLabel(ltr)),
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
            {
                return;
            }
#endif
            status = STATUS_WRITING;
            bCancel->setEnabled(true);
            bWrite->setEnabled(false);
            bRead->setEnabled(false);
            bVerify->setEnabled(false);
            double mbpersec;

            int compare_result;

            unsigned long long i, lasti, availablesectors, numsectors;
            int volumeID = cboxDevice->currentText().at(1).toLatin1() - 'A';
            int deviceID = cboxDevice->itemData(cboxDevice->currentIndex()).toInt();
            hVolume = getHandleOnVolume(volumeID, GENERIC_WRITE);
            if (hVolume == INVALID_HANDLE_VALUE)
            {
                status = STATUS_IDLE;
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            if (!getLockOnVolume(hVolume))
            {
                CloseHandle(hVolume);
                status = STATUS_IDLE;
                hVolume = INVALID_HANDLE_VALUE;
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            if (!unmountVolume(hVolume))
            {
                removeLockOnVolume(hVolume);
                CloseHandle(hVolume);
                status = STATUS_IDLE;
                hVolume = INVALID_HANDLE_VALUE;
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            hFile = getHandleOnFile(LPCWSTR(leFile->text().data()), GENERIC_READ);
            if (hFile == INVALID_HANDLE_VALUE)
            {
                removeLockOnVolume(hVolume);
                CloseHandle(hVolume);
                status = STATUS_IDLE;
                hVolume = INVALID_HANDLE_VALUE;
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            //hRawDisk = getHandleOnDevice(deviceID, GENERIC_WRITE);
            hRawDisk = getHandleOnDevice(deviceID, GENERIC_WRITE|GENERIC_READ);
            if (hRawDisk == INVALID_HANDLE_VALUE)
            {
                removeLockOnVolume(hVolume);
                CloseHandle(hFile);
                CloseHandle(hVolume);
                status = STATUS_IDLE;
                hVolume = INVALID_HANDLE_VALUE;
                hFile = INVALID_HANDLE_VALUE;
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            availablesectors = getNumberOfSectors(hRawDisk, &sectorsize);
            numsectors = getFileSizeInSectors(hFile, sectorsize);
            if (numsectors > availablesectors)
            {
                bool datafound = false;
                i = availablesectors;
                unsigned long nextchunksize = 0;
                while ( (i < numsectors) && (datafound == false) )
                {
                    nextchunksize = ((numsectors - i) >= 1024ul) ? 1024ul : (numsectors - i);
                    sectorData = readSectorDataFromHandle(hFile, i, nextchunksize, sectorsize);
                    if(sectorData == NULL)
                    {
                        // if there's an error verifying the truncated data, just move on to the
                        //  write, as we don't care about an error in a section that we're not writing...
                        i = numsectors + 1;
                    } else {
                        unsigned int j = 0;
                        unsigned limit = nextchunksize * sectorsize;
                        while ( (datafound == false) && ( j < limit ) )
                        {
                            if(sectorData[j++] != 0)
                            {
                                datafound = true;
                            }
                        }
                        i += nextchunksize;
                    }
                }
                // delete the allocated sectorData
                delete[] sectorData;
                sectorData = NULL;
                // build the string for the warning dialog
                std::ostringstream msg;
                msg << "More space required than is available:"
                    << "\n  Required: " << numsectors << " sectors"
                    << "\n  Available: " << availablesectors << " sectors"
                    << "\n  Sector Size: " << sectorsize
                    << "\n\nThe extra space " << ((datafound) ? "DOES" : "does not") << " appear to contain data"
                    << "\n\nContinue Anyway?";
                if(QMessageBox::warning(NULL, tr("Not enough available space!"),
                                        tr(msg.str().c_str()), QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok)
                {
                    // truncate the image at the device size...
                    numsectors = availablesectors;
                }
                else    // Cancel
                {
                    removeLockOnVolume(hVolume);
                    CloseHandle(hRawDisk);
                    CloseHandle(hFile);
                    CloseHandle(hVolume);
                    status = STATUS_IDLE;
                    hVolume = INVALID_HANDLE_VALUE;
                    hFile = INVALID_HANDLE_VALUE;
                    hRawDisk = INVALID_HANDLE_VALUE;
                    bCancel->setEnabled(false);
                    setReadWriteButtonState();
                    return;
                }
            }

            progressbar->setRange(0, (numsectors == 0ul) ? 100 : (int)numsectors);
            lasti = 0ul;
            update_timer.start();
            elapsed_timer->start();
            for (i = 0ul; i < numsectors && status == STATUS_WRITING; i += 1024ul)
            {
                sectorData = readSectorDataFromHandle(hFile, i, (numsectors - i >= 1024ul) ? 1024ul:(numsectors - i), sectorsize);
                if (sectorData == NULL)
                {
                    removeLockOnVolume(hVolume);
                    CloseHandle(hRawDisk);
                    CloseHandle(hFile);
                    CloseHandle(hVolume);
                    status = STATUS_IDLE;
                    hRawDisk = INVALID_HANDLE_VALUE;
                    hFile = INVALID_HANDLE_VALUE;
                    hVolume = INVALID_HANDLE_VALUE;
                    bCancel->setEnabled(false);
                    setReadWriteButtonState();
                    return;
                }
#if 0
                if (!writeSectorDataToHandle(hRawDisk, sectorData, i, (numsectors - i >= 1024ul) ? 1024ul:(numsectors - i), sectorsize))
                {
                    delete[] sectorData;
                    removeLockOnVolume(hVolume);
                    CloseHandle(hRawDisk);
                    CloseHandle(hFile);
                    CloseHandle(hVolume);
                    status = STATUS_IDLE;
                    sectorData = NULL;
                    hRawDisk = INVALID_HANDLE_VALUE;
                    hFile = INVALID_HANDLE_VALUE;
                    hVolume = INVALID_HANDLE_VALUE;
                    bCancel->setEnabled(false);
                    setReadWriteButtonState();
                    return;
                }
#endif
                sectorData2 = readSectorDataFromHandle(hRawDisk, i, (numsectors - i >= 1024ul) ? 1024ul:(numsectors - i), sectorsize);
                if (sectorData2 == NULL)
                {
                    delete sectorData2;
                    removeLockOnVolume(hVolume);
                    CloseHandle(hRawDisk);
                    CloseHandle(hFile);
                    CloseHandle(hVolume);
                    status = STATUS_IDLE;
                    sectorData2 = NULL;
                    hRawDisk = INVALID_HANDLE_VALUE;
                    hFile = INVALID_HANDLE_VALUE;
                    hVolume = INVALID_HANDLE_VALUE;
                    bCancel->setEnabled(false);
                    setReadWriteButtonState();
                    return;
                }

                compare_result = memcmp( sectorData, sectorData2, ((numsectors - i >= 1024ul) ? 1024ul:(numsectors - i) * sectorsize) );
                if (compare_result != 0)
                {
                    delete[] sectorData;
                    delete[] sectorData2;
                    sectorData  = NULL;
                    sectorData2 = NULL;
                    removeLockOnVolume(hVolume);
                    CloseHandle(hRawDisk);
                    CloseHandle(hFile);
                    CloseHandle(hVolume);
                    status = STATUS_IDLE;
                    sectorData2 = NULL;
                    hRawDisk = INVALID_HANDLE_VALUE;
                    hFile = INVALID_HANDLE_VALUE;
                    hVolume = INVALID_HANDLE_VALUE;
                    bCancel->setEnabled(false);
                    setReadWriteButtonState();
                    QMessageBox::critical(NULL, tr("Verification Error"), tr("Read back data is different from original data."));
                    return;
                }

                delete[] sectorData;
                sectorData = NULL;

                delete[] sectorData2;
                sectorData2 = NULL;

                QCoreApplication::processEvents();
                if (update_timer.elapsed() >= ONE_SEC_IN_MS)
                {
                    mbpersec = (((double)sectorsize * (i - lasti)) * ((float)ONE_SEC_IN_MS / update_timer.elapsed())) / 1024.0 / 1024.0;
                    statusbar->showMessage(QString("%1MB/s").arg(mbpersec));
                    elapsed_timer->update(i, numsectors);
                    update_timer.start();
                    lasti = i;
                }
                progressbar->setValue(i);
                QCoreApplication::processEvents();
            }
            removeLockOnVolume(hVolume);
            CloseHandle(hRawDisk);
            CloseHandle(hFile);
            CloseHandle(hVolume);
            hRawDisk = INVALID_HANDLE_VALUE;
            hFile = INVALID_HANDLE_VALUE;
            hVolume = INVALID_HANDLE_VALUE;
            if (status == STATUS_CANCELED){
                passfail = false;
            }
        }
        else if (!fileinfo.exists() || !fileinfo.isFile())
        {
            QMessageBox::critical(NULL, tr("File Error"), tr("The selected file does not exist."));
            passfail = false;
        }
        else if (!fileinfo.isReadable())
        {
            QMessageBox::critical(NULL, tr("File Error"), tr("You do not have permision to read the selected file."));
            passfail = false;
        }
        else if (fileinfo.size() == 0)
        {
            QMessageBox::critical(NULL, tr("File Error"), tr("The specified file contains no data."));
            passfail = false;
        }
        progressbar->reset();
        statusbar->showMessage(tr("Done."));
        bCancel->setEnabled(false);
        setReadWriteButtonState();
        if (passfail){
            QMessageBox::information(NULL, tr("Complete"), tr("Verify Successful."));
        }

    }
    else
    {
        QMessageBox::critical(NULL, tr("File Error"), tr("Please specify an image file to use."));
    }
    if (status == STATUS_EXIT)
    {
        close();
    }
    status = STATUS_IDLE;
    elapsed_timer->stop();
    */
}


WindowsDiskManager::~WindowsDiskManager()
{
    // TODO: Convert mainwindow destructor to WindowsDiskManager dtor
    if (mRawDiskHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(mRawDiskHandle);
        mRawDiskHandle = INVALID_HANDLE_VALUE;
    }
    if (mFileHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(mFileHandle);
        mFileHandle = INVALID_HANDLE_VALUE;
    }
    if (mVolumeHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(mVolumeHandle);
        mVolumeHandle = INVALID_HANDLE_VALUE;
    }
    if (sectorData != NULL)
    {
        delete[] sectorData;
        sectorData = NULL;
    }
}
