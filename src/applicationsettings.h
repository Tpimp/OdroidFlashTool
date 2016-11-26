#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QObject>

class QString;

class ApplicationSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool mFlashMode READ flashMode WRITE setFlashMode NOTIFY flashModeChanged)
    Q_PROPERTY(bool mUseCompression READ useCompression WRITE setUseCompression NOTIFY useCompressionChanged)
    Q_PROPERTY(bool mDeleteAfterDecompress READ deleteAfterDecompress WRITE setDeleteAfterDecompress NOTIFY deleteAfterDecompressChanged)
    Q_PROPERTY(bool mDeleteAfterCompress READ deleteAfterCompress   WRITE setDeleteAfterCompress   NOTIFY deleteAfterCompressChanged)
    Q_PROPERTY(bool mVerifyFlash READ verifyFlash   WRITE setVerifyFlash   NOTIFY verifyFlashChanged)
    Q_PROPERTY(bool mPartitionsOnly READ usePartitionsOnly WRITE setUsePartitionsOnly NOTIFY usePartitionsOnlyChanged)
    Q_PROPERTY(bool mShowCompressDialog READ showCompressDialog WRITE setShowCompressDialog NOTIFY showCompressDialogChanged)
    Q_PROPERTY(bool mShowDecompressDialog READ showDecompressDialog WRITE setShowDecompressDialog NOTIFY showDecompressDialogChanged)
    Q_PROPERTY(bool mShowNotReadyDisks READ showNotReadyDisks WRITE setShowNotReadyDisks NOTIFY showNotReadyDisksChanged)
    Q_PROPERTY(bool mWriteBootIni READ writeBootIni WRITE setWriteBootIni NOTIFY writeBootIniChanged)
    Q_PROPERTY(bool mPreserveBootIni READ preserveBootIni WRITE setPreserveBootIni NOTIFY writeBootIniChanged)
    //Q_PROPERTY(bool mIsFlashDisk READ isFlashDisk WRITE setIsFlashDisk NOTIFY isFlashDiskChanged)
    Q_PROPERTY(QString mWorkDirectory READ workDirectory WRITE setWorkDirectory NOTIFY workDirectoryChanged)

public:
    explicit ApplicationSettings(QObject *parent = 0);
    bool flashMode();
    bool shouldCompress();
    bool deleteAfterDecompress();
    bool deleteAfterCompress();
    QString lastBootIniBackup();
    bool verifyFlash();
    bool useCompression();
    bool usePartitionsOnly();
    bool preserveBootIni();
    bool showCompressDialog();
    bool showDecompressDialog();
    bool createSettings(QString settings_path);
    bool showNotReadyDisks();
    bool writeBootIni();
    Q_INVOKABLE bool loadSettings(QString settings_path);
    Q_INVOKABLE bool saveSettings();
    QString workDirectory();
    Q_INVOKABLE bool settingsFileExist(QString file_path);
    void setLastBackupBootIni(QString boot_ini);

signals:
    void flashModeChanged(bool flash_mode);
    void useCompressionChanged(bool use_compression);
    void deleteAfterDecompressChanged(bool delete_after);
    void deleteAfterCompressChanged(bool delete_after);
    void preserveBootIniChanged(bool preserve_boot);
    void verifyFlashChanged(bool verify_flash);
    void usePartitionsOnlyChanged(bool use_partitions);
    void showCompressDialogChanged(bool show_dialog);
    void showDecompressDialogChanged(bool show_dialog);
    void showNotReadyDisksChanged(bool show_not_ready);
    void settingsChanged();
    void settingsLoaded();
    void settingsSaved();
    void failedToLoadOrSave(bool saving, QString error_str);
    void workDirectoryChanged(QString work_directory);
    void writeBootIniChanged(bool write_boot);

public slots:
    void setFlashMode(bool flash_mode);
    void setUseCompression(bool use_compression);
    void setDeleteAfterDecompress(bool delete_after);
    void setDeleteAfterCompress(bool delete_after);
    void setPreserveBootIni(bool preserve_boot);
    void setVerifyFlash(bool verify_flash);
    void setUsePartitionsOnly(bool use_partitions);
    void setShowCompressDialog(bool show_dialog);
    void setShowDecompressDialog(bool show_dialog);
    void setShowNotReadyDisks(bool show_not_ready);
    void setWorkDirectory(QString work_directory);
    void setWriteBootIni(bool write_boot);

private:
    bool    mFlashMode = true;              // READ or WRITE 0 or 1
    bool    mUseCompression = false;        // Should Decompress on Write or Compress after Read
    bool    mDeleteAfterDecompress = true;  // Should Delete image after decompressed from .xz
    bool    mDeleteAfterCompress = true;    // Should Delete image after it is compressed
    bool    mVerifyFlash = false;           // Use block verification when writing and reading
    bool    mPartitionsOnly = true;         // Flash or Copy partitions only
    bool    mShowNotReadyDisks = false;     // determines to show empty removable drives
    bool    mWriteBootIni = false;          // Flag to write bootini changes
    bool    mPreserveBootIni = false;       // Will store and back up and end
    QString mWorkDirectory;                 // Stores the path

    // booleans for controlling the behavior of UI
    bool    mShowCompressDialog = true;
    bool    mShowDecompressDialog = true;

    // non-property internals
    QString mLoadedPath;
    QString mLastBackupBootIni;
    bool    mIsFlashDisk;
};

#endif // APPLICATIONSETTINGS_H
