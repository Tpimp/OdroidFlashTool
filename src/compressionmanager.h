#ifndef COMPRESSIONMANAGER_H
#define COMPRESSIONMANAGER_H

#include <QObject>
class DecompressionThread;
class CompressionThread;
class CompressionManager : public QObject
{
    Q_OBJECT
public:
    explicit CompressionManager(QObject *parent = 0);
    ~CompressionManager();
signals:

public slots:
    void decompressFile(QString archive_path, QString decompress_path, quint64 memoryLimit);
private:
    DecompressionThread * mDecompressor = nullptr;
    QString               mDecompressTempDir;
    QString               mCompressTempDir;
private slots:
    void cleanUpDecompressor();
};

#endif // COMPRESSIONMANAGER_H
