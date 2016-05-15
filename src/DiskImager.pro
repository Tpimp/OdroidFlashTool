###################################################################
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, see http://gnu.org/licenses/
#
#
#  Copyright (C) 2009, Justin Davis <tuxdavis@gmail.com>
#  Copyright (C) 2009-2014 ImageWriter developers
#                          https://launchpad.net/~image-writer-devs
###################################################################
TEMPLATE = app
TARGET = ../../OdroidFlashTool
DEPENDPATH += .
INCLUDEPATH += .
#CONFIG += release
QT += qml quick
#DEFINES += QT_NO_CAST_FROM_ASCII
VERSION = 0.1.0
VERSTR = '\\"$${VERSION}\\"'
DEFINES += VER=\"$${VERSTR}\"
DEFINES += WINVER=0x0601
DEFINES += _WIN32_WINNT=0x0601
QMAKE_TARGET_PRODUCT = "Odroid Flash Tool"
QMAKE_TARGET_DESCRIPTION = "Writes image files to microSD and eMMC cards"

# Input
HEADERS += disk.h\
    diskimager.h \
    windowsdiskmanager.h \
    linuxdiskmanager.h \
    odroidflashmanager.h \
    compressionmanager.h \
    decompressionthread.h


SOURCES += disk.cpp\
           main.cpp\
    diskimager.cpp \
    windowsdiskmanager.cpp \
    linuxdiskmanager.cpp \
    odroidflashmanager.cpp \
    compressionmanager.cpp \
    decompressionthread.cpp


#RC_FILE = DiskImager.rc

TRANSLATIONS  = diskimager_en.ts\
                diskimager_cn.ts\
                diskimager_it.ts\
                diskimager_pl.ts

RESOURCES += \
    qml.qrc
