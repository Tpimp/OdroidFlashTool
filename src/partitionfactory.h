#ifndef PARTITIONFACTORY_H
#define PARTITIONFACTORY_H

#include <QObject>

#pragma pack(1)

struct EBR_RECORD
{
    unsigned int  BootIndicator : 8;
    unsigned int  HeadStart : 24;
    unsigned int  PartitionType : 8;
    unsigned int  HeadEnd : 24;
    unsigned int  LBAStart : 32;
    unsigned int  LBASize : 32;
    EBR_RECORD& operator =(const EBR_RECORD& right)
    {
        memcpy(const_cast<EBR_RECORD*>(this),&right ,sizeof(EBR_RECORD));
        return *this;
    }
};

namespace EBR_CONSTANTS
{
    const uint16_t SIZE_OF_AAP = 446;
    const uint16_t EBR_SIGNATURE = 0x55AA;
}


struct EBR_TABLE
{
    uint8_t    AAP[EBR_CONSTANTS::SIZE_OF_AAP] = {0};
    EBR_RECORD Record1;
    EBR_RECORD Record2;
    EBR_RECORD Record3;
    EBR_RECORD Record4;
    uint16_t   Signature  = EBR_CONSTANTS::EBR_SIGNATURE; // magic signature
    EBR_TABLE& operator =(const EBR_TABLE& right)
    {
        memcpy(const_cast<EBR_TABLE*>(this), &right,sizeof(EBR_TABLE));
        return *this;
    }
    void setData(char * data, int length)
    {
        if(length > 512 || length < 0 || data == nullptr)
            return;
        memcpy(const_cast<EBR_TABLE*>(this),data,length);
    }
};
namespace MBR_CONSTANTS
{
    const uint8_t BOOTSTRAP_SIZE = 218;
    const uint8_t BOOTSTRAP2_SIZE = 216;
}
struct MBR_RECORD
{
    uint8_t  BootFlag             :8;
    uint8_t  CHSStartHead         :8;
    uint8_t  CHSStartCylinderHigh :8;
    uint8_t  CHSStartCylinderLow  :8;
    uint8_t  PartitionType        :8;
    uint8_t  CHSEndHead           :8;
    uint8_t  CHSEndCylinderHigh   :8;
    uint8_t  CHSEndCylinderLow    :8;
    uint32_t LBAAddress           :32;
    uint32_t PartitionSectors     :32;

};
struct MBR_TABLE
{
    uint8_t    BootStrap[MBR_CONSTANTS::BOOTSTRAP_SIZE];
    uint16_t   TimeStampHeader : 16;
    uint8_t    OriginalPhysicalDrive : 8;
    uint8_t    TimeStampSeconds : 8;
    uint8_t    TimeStampMinutes : 8;
    uint8_t    TimeStampHours : 8;
    uint8_t    BootStrap2[MBR_CONSTANTS::BOOTSTRAP2_SIZE];
    uint32_t   DiskOSSignature : 32;
    uint16_t   DiskOSPermissions = 0x0000;
    MBR_RECORD Record1;
    MBR_RECORD Record2;
    MBR_RECORD Record3;
    MBR_RECORD Record4;
    uint16_t   BootSignature = EBR_CONSTANTS::EBR_SIGNATURE;
    void setData(char * data, int length)
    {
        if(length > 512 || length < 0 || data == nullptr)
            return;
        memcpy(const_cast<MBR_TABLE*>(this),data,length);
    }
};


struct GPT_HEADER{
    uint64_t  Signature  : 64;
    uint32_t  Revision   : 32;
    uint32_t  HeaderSize : 32;
    uint32_t  CRC32Header: 32;
    uint32_t  RESERVED = 0;
    uint64_t  CurrentLBA : 64;
    uint64_t  BackupLBA  : 64;
    uint64_t  FirstLBA   : 64; // Primary Partition table last LBA + 1
    uint64_t  LastLBA    : 64; // Secondary Partition table first LBA - 1
    uint8_t   DiskGUID[16];
    uint64_t  PartitionsStartingLBA : 64;
    uint32_t  PartitionCount : 32;
    uint32_t  PartitionSize  : 32;
    uint32_t  CRC32Partition : 32;
    uint8_t   RESERVED2[420];


};

struct GPT_RECORD{
    uint8_t   GUIDType[16];
    uint8_t   UUID[16];
    uint64_t  FirstLBA    : 64;
    uint64_t  LastLBA     : 64;
    uint64_t  Attributes  : 64;
    uint16_t  TypeName[36];
};

struct GPT_TABLE{
    MBR_RECORD   MBR;
    GPT_HEADER   GPTHeader;
    GPT_RECORD   Partitions[128];
    GPT_RECORD   BackupPartitions[128];
    GPT_HEADER   BackupHeader;
    void setData(char * data, int length)
    {
        if(length > 33808 || length < 0 || data == nullptr)
            return;
        memcpy(const_cast<GPT_TABLE*>(this),data,length);
    }
};


struct C1_BOOT_TABLE{ // high level
    uint8_t   MBR_BL1[32000]; // BL1 AMLOGIC Binary - 32kB
    uint8_t   UBOOT_BIN[480000]; // Bootloader Binary - 480kB
    uint8_t   UBOOT_ENV[3200];   // Stores UBOOT Environment CRC32 checksum - 32kB
};

struct C2_BOOT_TABLE{
    uint8_t   MBR_BL1[48500]; // BL1 AMLOGIC Binary                          - 48.5kB
    uint8_t   UBOOT_BIN[667500]; // Bootloader Binary                        - 667.5kB
    uint8_t   UBOOT_ENV[3200];   // Stores UBOOT Environment CRC32 checksum  - 32kB
};

struct XU4_BOOT_TABLE{
    MBR_TABLE MBR;  // Standard MBR table - 512B
    uint8_t   FW_BLOB1[15000]; // Firmware Blob 1 (Samsung) - 15kB
    uint8_t   FW_BLOB2[16000]; // Firmware Blob 2 (Odroid)  - 16kB
    uint8_t   UBOOT_BIN[328000]; // UBOOT Binary            - 328kB
    uint8_t   ARM_TZSW[256000]; // Arm Trust Zone SW        - 256kB
    uint8_t   UBOOT_ENV[16000];   // Stores UBOOT Environment CRC32 checksum - 16kB
};

typedef  XU4_BOOT_TABLE U3_BOOT_TABLE; // U3 Boot table and Xu4 are identical



//TODO: Add partition struct for FAT16 and FAT32

#pragma pack()


class PartitionFactory : public QObject
{
    Q_OBJECT

public:
    explicit PartitionFactory(QObject *parent = 0);

signals:
    void partitionTableInfoLoaded(QByteArray data);
    void errorLoadingTableInfo(QString btd_file, int error);

public slots:
    void loadPartitionTableInfo(QString btd_file); // loads in expected
};

#endif // PARTITIONFACTORY_H
