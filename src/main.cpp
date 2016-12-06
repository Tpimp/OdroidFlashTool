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
 *                          https://launchpad.net/~image-writer-devs  *
 **********************************************************************/

#ifndef WINVER
#define WINVER 0x0601
#endif

#include <QGuiApplication>
#include "applicationsettings.h"
#include "odroidflashmanager.h"
#include <QCommandLineParser>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>
#include <QtQml>
#include "diskmanager.h"
#ifdef Q_OS_WIN
    #include "windowsdiskmanager.h"
#else

#endif
int main(int argc, char *argv[])
{
    QCommandLineParser parser;
    // Add command line parsing rules

    bool isGraphical(true);
    QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load(QString("diskimager_")+ locale);


    if(isGraphical)
    {
        QGuiApplication app(argc, argv);
        app.setApplicationDisplayName("Odroid Flash Tool");
        QQmlApplicationEngine graphical_engine(&app);
        QString reason("??");
        qmlRegisterUncreatableType<OdroidFlashManager>("com.odroid.odf",1,0,"FlashManager",reason);

        app.installTranslator(&translator);
        DiskManager  *    disk_manager;
        #ifdef Q_OS_WIN
            WindowsDiskManager* dm(new WindowsDiskManager(&app));
            disk_manager = reinterpret_cast<DiskManager*>(dm);
        #else

        #endif

        QString pwd = app.applicationDirPath();
        ApplicationSettings   settings(&app);

        if(!settings.settingsFileExist(pwd +"/" + "settings.json"))
        {
            QString temp_dir = pwd;
            temp_dir.append("/tmp");
            settings.setWorkDirectory(temp_dir);
            settings.createSettings(pwd +"/settings.json");
        }
        else
            settings.loadSettings(pwd + "/settings.json");
        OdroidFlashManager * flashmanager = new OdroidFlashManager(disk_manager,&settings,pwd,&app);

        graphical_engine.rootContext()->setContextProperty("ODF",flashmanager);
        graphical_engine.rootContext()->setContextProperty("AppSettings",&settings);
        graphical_engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
        return app.exec();
    }
    else
    {
        // Handle commandline system

    }
    return 0;

}
