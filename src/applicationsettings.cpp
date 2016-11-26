#include "applicationsettings.h"
#include <QFile>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonParseError>
#include <QString>
#include <QDir>
ApplicationSettings::ApplicationSettings(QObject *parent) : QObject(parent){}

bool ApplicationSettings::createSettings(QString settings_path)
{
    QFile settings_file(settings_path);
    if(!settings_file.exists())
    {
        settings_file.open(QIODevice::WriteOnly);
        QJsonObject obj;
        obj["DAD"] = QJsonValue(mDeleteAfterDecompress);
        obj["DAC"] = QJsonValue(mDeleteAfterCompress);
        obj["PARTS"] = QJsonValue(mPartitionsOnly);
        obj["SCD"] = QJsonValue(mShowCompressDialog);
        obj["SDD"] = QJsonValue(mShowDecompressDialog);
        obj["PWD"] = QJsonValue(mWorkDirectory);
        obj["SNR"] = QJsonValue(mShowNotReadyDisks);
        QJsonDocument doc;
        doc.setObject(obj);
        settings_file.write(doc.toJson());
        settings_file.flush();
    }
    return true;
}



bool ApplicationSettings::flashMode()
{
    return mFlashMode;
}

bool ApplicationSettings::shouldCompress()
{
    return mUseCompression;
}

bool ApplicationSettings::deleteAfterDecompress()
{
    return mDeleteAfterDecompress;
}

bool ApplicationSettings::deleteAfterCompress()
{
    return mDeleteAfterCompress;
}

QString ApplicationSettings::lastBootIniBackup()
{
    return mLastBackupBootIni;
}

bool ApplicationSettings::preserveBootIni()
{
    return mPreserveBootIni;
}

bool ApplicationSettings::verifyFlash()
{
    return mVerifyFlash;
}

bool ApplicationSettings::useCompression()
{
    return mUseCompression;
}

bool ApplicationSettings::usePartitionsOnly()
{
    return mPartitionsOnly;
}

bool ApplicationSettings::showCompressDialog()
{
    return mShowCompressDialog;
}

bool ApplicationSettings::showDecompressDialog()
{
    return mShowDecompressDialog;
}

bool ApplicationSettings::showNotReadyDisks()
{
    return mShowNotReadyDisks;
}


QString ApplicationSettings::workDirectory()
{
    return mWorkDirectory;
}

bool ApplicationSettings::writeBootIni()
{
    return mWriteBootIni;
}

bool ApplicationSettings::settingsFileExist(QString file_path)
{
    if(file_path.isEmpty())
        return false;

    QFile settings_file(file_path);
    return settings_file.exists();
}

void ApplicationSettings::setFlashMode(bool flash_mode)
{
    if(flash_mode != mFlashMode)
    {
        mFlashMode = flash_mode;
        emit flashModeChanged(flash_mode);
        emit settingsChanged();
    }
}
void ApplicationSettings::setUseCompression(bool use_compression)
{
    if(use_compression != mUseCompression)
    {
        mUseCompression = use_compression;
        emit useCompressionChanged(use_compression);
        emit settingsChanged();
    }
}

void ApplicationSettings::setDeleteAfterCompress(bool delete_after)
{
    if(delete_after != mDeleteAfterCompress)
    {
        mDeleteAfterCompress = delete_after;
        emit deleteAfterCompressChanged(delete_after);
        emit settingsChanged();
    }
}

void ApplicationSettings::setDeleteAfterDecompress(bool delete_after)
{
    if(delete_after != mDeleteAfterDecompress)
    {
        mDeleteAfterDecompress = delete_after;
        emit deleteAfterDecompressChanged(delete_after);
        emit settingsChanged();
    }
}


void ApplicationSettings::setPreserveBootIni(bool preserve_boot)
{
    if(preserve_boot != mPreserveBootIni)
    {
        mPreserveBootIni = preserve_boot;
        emit preserveBootIniChanged(preserve_boot);
        emit settingsChanged();
    }
}

void ApplicationSettings::setUsePartitionsOnly(bool use_partitions)
{
    if(use_partitions != mPartitionsOnly)
    {
        mPartitionsOnly = use_partitions;
        emit usePartitionsOnlyChanged(use_partitions);
        emit settingsChanged();
    }
}

void ApplicationSettings::setShowCompressDialog(bool show_dialog)
{
    if(show_dialog != mShowCompressDialog)
    {
        mShowCompressDialog = show_dialog;
        emit showCompressDialogChanged(show_dialog);
        emit settingsChanged();
    }
}

void ApplicationSettings::setShowDecompressDialog(bool show_dialog)
{
    if(show_dialog != mShowDecompressDialog)
    {
        mShowDecompressDialog = show_dialog;
        emit showDecompressDialogChanged(show_dialog);
        emit settingsChanged();
    }
}

void ApplicationSettings::setShowNotReadyDisks(bool show_not_ready)
{
    if(show_not_ready != mShowNotReadyDisks)
    {
        mShowNotReadyDisks = show_not_ready;
        emit showNotReadyDisksChanged(show_not_ready);
        emit settingsChanged();
    }
}

void ApplicationSettings::setVerifyFlash(bool verify_flash)
{
    if(verify_flash != mVerifyFlash)
    {
        mVerifyFlash = verify_flash;
        emit verifyFlashChanged(verify_flash);
        emit settingsChanged();
    }
}

void ApplicationSettings::setWriteBootIni(bool write_boot)
{
    if(write_boot != mWriteBootIni)
    {
        mWriteBootIni = write_boot;
        emit writeBootIniChanged(write_boot);
        emit settingsChanged();
    }
}

bool ApplicationSettings::loadSettings(QString settings_path)
{
    QFile settings_file(settings_path);
    if(!settings_file.exists())
    {
        mLoadedPath.clear();
        return false;
    }
    mLoadedPath = settings_path;
    settings_file.open(QIODevice::ReadOnly);
    QByteArray data = settings_file.readAll();
    QJsonDocument  doc;
    QJsonParseError errored;
    doc = QJsonDocument::fromJson(data,&errored);
    if(errored.error != QJsonParseError::NoError)
    {
        QString error_str("Errored while parsing the settings file " + mLoadedPath);
        // TODO switch on the error value and build a proper error string
        emit failedToLoadOrSave(false,error_str);
        return false;
    }
    QJsonObject main;
    main = doc.object();
    mDeleteAfterDecompress = main["DAD"].toBool();
    mDeleteAfterCompress = main["DAC"].toBool();
    mPartitionsOnly = main["PARTS"].toBool();
    mShowCompressDialog = main["SCD"].toBool();
    mShowDecompressDialog = main["SDD"].toBool();
    mWorkDirectory = main["PWD"].toString();
    mShowNotReadyDisks = main["SNR"].toBool();
    emit settingsLoaded();
    return true;
}

bool ApplicationSettings::saveSettings()
{
    if(mLoadedPath.isEmpty())
    {
        QString error_str(QStringLiteral("No settings file path was set, please set one"));
        emit failedToLoadOrSave(true,error_str);
        return false;
    }

    QFile settings_file(mLoadedPath);
    if(!settings_file.exists())
    {
        QString error_str(QStringLiteral("No settings file found at the path ") +mLoadedPath);
        emit failedToLoadOrSave(true,error_str);
        return false;
    }
    settings_file.open(QIODevice::WriteOnly);
    QJsonObject obj;

    obj["DAD"] = QJsonValue(mDeleteAfterDecompress);
    obj["DAC"] = QJsonValue(mDeleteAfterCompress);
    obj["PARTS"] = QJsonValue(mPartitionsOnly);
    obj["SCD"] = QJsonValue(mShowCompressDialog);
    obj["SDD"] = QJsonValue(mShowDecompressDialog);
    obj["PWD"] = QJsonValue(mWorkDirectory);
    obj["SNR"] = QJsonValue(mShowNotReadyDisks);
    QJsonDocument doc;
    doc.setObject(obj);
    settings_file.write(doc.toJson());
    settings_file.flush();
    emit settingsSaved();
    return true;
}


void ApplicationSettings::setLastBackupBootIni(QString boot_ini)
{
    mLastBackupBootIni = boot_ini;
}

void ApplicationSettings::setWorkDirectory(QString work_directory)
{
    if(mWorkDirectory.compare(work_directory) != 0)
    {
        mWorkDirectory = work_directory;
        emit workDirectoryChanged(work_directory);
        emit settingsChanged();
    }
}



