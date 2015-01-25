//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2015 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#ifndef APP_H
#define APP_H

#include <QSettings>
#include <QApplication>
#include <QSimpleUpdater>

#include "window.h"

class Application : public QApplication
{
        Q_OBJECT

    public:
        Application (int &argc, char **argv);

    public slots:
        void checkForUpdates (void);

    private slots:
        void setupUpdater (void);
        void showWelcomeMessages (void);

    protected:
        bool event (QEvent *_event);

    private:
        Window *m_window;
        QSettings *m_settings;
        QSimpleUpdater *m_updater;

        bool m_show_all_updater_messages;
};

#endif
