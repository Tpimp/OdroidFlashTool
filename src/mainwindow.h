/**********************************************************************
 *  This program is free software; you can redistribute it and/or     *
 *  modify it under the terms of the GNU General Public License       *
 *  as published by the Free Software Foundation; either version 2    *
 *  of the License, or (at your option) any later version.            *
 *                                                                    *
 *  This program is distributed in the hope that it will be useful,   *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of    *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the     *
 *  GNU General Public License for more details.                      *
 *                                                                    *
 *  You should have received a copy of the GNU General Public License *
 *  along with this program; if not, see http://gnu.org/licenses/
 *  ---                                                               *
 *  Copyright (C) 2009, Justin Davis <tuxdavis@gmail.com>             *
 *  Copyright (C) 2009-2014 ImageWriter developers                    *
 *                          http://launchpad.net/~image-writer-devs   *
 **********************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifndef WINVER
#define WINVER 0x0601
#endif

#include <QtWidgets>
#include <QClipboard>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <winioctl.h>
#include "ui_mainwindow.h"
#include "disk.h"
#include "elapsedtimer.h"
#define DONT_VERIFY_WRITE 1

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT
    public:
        MainWindow(QWidget *parent=0);
        ~MainWindow();
        void closeEvent(QCloseEvent *event);
        enum Status {STATUS_IDLE=0, STATUS_READING, STATUS_WRITING, STATUS_EXIT, STATUS_CANCELED, STATUS_VERIFYING,};
        bool nativeEvent(const QByteArray &type, void *vMsg, long *result);
    protected slots:
        void on_tbBrowse_clicked();
        void on_bCancel_clicked();
        void on_bWrite_clicked();
        void on_bVerify_clicked();
        void on_bRead_clicked();
        void on_leFile_textChanged();
        void on_leFile_editingFinished();
        void on_md5CheckBox_stateChanged();
        void on_bMd5Copy_clicked();
    private:
        // find attached devices
        void getLogicalDrives();
        void setReadWriteButtonState();
        //void setReadWriteVerifyButtonState();

        HANDLE hVolume;
        HANDLE hFile;
        HANDLE hRawDisk;
        static const unsigned short ONE_SEC_IN_MS = 1000;
        unsigned long long sectorsize;
        int status;
        char *sectorData;
//#ifndef DONT_VERIFY_WRITE
        char *sectorData2;
//#endif
        QTime update_timer;
        ElapsedTimer *elapsed_timer = NULL;
        QClipboard *clipboard;
        void generateMd5(char *filename);
        QString myHomeDir;
        void updateMd5CopyButton();
};

#endif // MAINWINDOW_H
