#ifndef WINDOWSDISKMANAGER_H
#define WINDOWSDISKMANAGER_H

#include <QObject>
#include <QJsonObject>

class WindowsDiskManager : public QObject
{
    Q_OBJECT
public:
    explicit WindowsDiskManager(QObject *parent = 0);

signals:

public slots:
    void  queryDiskLocations();
    void  queryBootIni();
    void  requestWriteImage(QString image_path, QString write_path);
    void  requestReadImage(QString read_path, QString image_path);

private:// windows specific members (windows handles,...)

};

#endif // WINDOWSDISKMANAGER_H
