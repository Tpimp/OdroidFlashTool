#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <QObject>

class ImageManager : public QObject
{
    Q_OBJECT
public:
    explicit ImageManager(QObject *parent = 0);

signals:

public slots:
};

#endif // IMAGEMANAGER_H