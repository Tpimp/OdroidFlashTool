TEMPLATE = app
TARGET = OdroidFlashTool
DEPENDPATH += .
INCLUDEPATH += .
#CONFIG += release
QT += qml quick
VERSION = 0.1.0
VERSTR = '\\"$${VERSION}\\"'

QMAKE_TARGET_PRODUCT = "Odroid Flash Tool"
QMAKE_TARGET_DESCRIPTION = "Writes image files to microSD and eMMC cards"
# Input
HEADERS += \
    linuxdiskmanager.h \
    odroidflashmanager.h \
    compressionmanager.h \
    decompressionthread.h \
    compressionthread.h \
    windowsdiskreaderthread.h \
    windowsdiskwriterthread.h \
    partitionfactory.h \
    applicationsettings.h \
    bootmanager.h \
    imagemanager.h \
    diskmanager.h \
    windowsdiskutility.h


SOURCES += main.cpp\
    linuxdiskmanager.cpp \
    odroidflashmanager.cpp \
    compressionmanager.cpp \
    decompressionthread.cpp \
    compressionthread.cpp \
    windowsdiskreaderthread.cpp \
    windowsdiskwriterthread.cpp \
    partitionfactory.cpp \
    applicationsettings.cpp \
    bootmanager.cpp \
    imagemanager.cpp

win32 {
    DEFINES += VER=\"$${VERSTR}\"
    DEFINES += WINVER=0x0601
    DEFINES += _WIN32_WINNT=0x0601
    HEADERS += windowsdiskmanager.h


    SOURCES += windowsdiskmanager.cpp

    RC_ICONS = release/OdroidFlashTool.ico
    RC_FILE = OdroidFlashTool_resource.rc
    #CONFIG += embed_manifest_exe
    #QMAKE_LFLAGS_WINDOWS += MANIFESTUAC:level=\'requireAdministrator\'
}


TRANSLATIONS  = diskimager_en.ts\
                diskimager_cn.ts\
                diskimager_it.ts\
                diskimager_pl.ts

RESOURCES += \
    qml.qrc \
    images.qrc \
    sounds.qrc

DISTFILES += \
    installer/config/config.xml \
    installer/packages/com.odroid.flashtool/meta/package.xml \
    installer/packages/com.odroid.flashtool/meta/license.txt \
    installer/packages/com.odroid.flashtool/meta/installscript.qs \
    images/Win32DiskImager.ico \
    odroidflashtool.qmodel

FORMS += \
    installer/resources/shortcuts.ui \
    installer/packages/com.odroid.flashtool/meta/shortcuts.ui
