//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#ifndef APP_H
#define APP_H

#include "window.h"

#include <QMessageBox>
#include <QApplication>
#include <QFileOpenEvent>

#include <QSimpleUpdater>

/*!
 * \class Application
 *
 * The \c Application class is in charge of loading and configuring
 * a new \c Window when Thunderpad is launched or when the system
 * requests Thunderpad to open a file (for example, when the user selects
 * "Thunderpad" from the Open with... menu in the file manager).
 */

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
        QSimpleUpdater *m_updater;
};

#endif
