#include "partitionfactory.h"

PartitionFactory::PartitionFactory(QObject *parent) : QObject(parent)
{

}


void PartitionFactory::loadPartitionTableInfo(QString btd_file)
{
    // Load in the btd_file (json format) and notify raw data ready
    QByteArray data;
    // find file, or report error
    // load the file into data or report error
    emit partitionTableInfoLoaded(data);
}
