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
HEADERS += diskimager.h \
    linuxdiskmanager.h \
    odroidflashmanager.h \
    compressionmanager.h \
    decompressionthread.h \
    compressionthread.h


SOURCES += main.cpp\
    diskimager.cpp \
    linuxdiskmanager.cpp \
    odroidflashmanager.cpp \
    compressionmanager.cpp \
    decompressionthread.cpp \
    compressionthread.cpp

win32 {
    DEFINES += VER=\"$${VERSTR}\"
    DEFINES += WINVER=0x0601
    DEFINES += _WIN32_WINNT=0x0601
    HEADERS += windowsdiskmanager.h \
               disk.h
    SOURCES += windowsdiskmanager.cpp \
               disk.cpp
    RC_ICONS = release/OdroidFlashTool.ico
	RC_FILE = OdroidFlashTool_resource.rc
}


TRANSLATIONS  = diskimager_en.ts\
                diskimager_cn.ts\
                diskimager_it.ts\
                diskimager_pl.ts

RESOURCES += \
    qml.qrc \
    images.qrc

DISTFILES += \
    installer/config/config.xml \
    installer/packages/com.odroid.flashtool/meta/package.xml \
    installer/packages/com.odroid.flashtool/meta/license.txt \
    installer/packages/com.odroid.flashtool/meta/installscript.qs \
    images/Win32DiskImager.ico

FORMS += \
    installer/resources/shortcuts.ui \
    installer/packages/com.odroid.flashtool/meta/shortcuts.ui
