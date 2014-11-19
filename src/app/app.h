//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#ifndef APP_H
#define APP_H

#include "window.h"

#include <QSettings>
#include <QMessageBox>
#include <QApplication>
#include <QFileOpenEvent>

#include <QSimpleUpdater>

class Application : public QApplication
{
        Q_OBJECT

    public:
        Application (int &argc, char **argv);
        int showInitError();

    public slots:
        void checkForUpdates();

    private slots:
        void showLatestVersion();
        void onCheckingFinished();
        void showUpdateAvailable();

    protected:
        bool event (QEvent *_event);

    private:
        Window *m_window;
        QSettings *m_settings;
        QSimpleUpdater *m_updater;

        bool m_show_all_updater_messages;
};

#endif
