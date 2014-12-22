//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#ifndef APP_H
#define APP_H

#include <QSettings>
#include <QMessageBox>
#include <QApplication>
#include <QSharedMemory>
#include <QFileOpenEvent>
#include <QSimpleUpdater>

#include "window.h"

class Application : public QApplication {
    Q_OBJECT

public:
    Application(int &argc, char **argv);

    /*!
     * \brief showInitError
     * \return
     *
     * Shows a message box informing the user
     * that there was an error connecting to
     * the current instance of the application.
     */

    int showInitError(void);

public slots:

    /*!
     * \brief checkForUpdates
     *
     * Connects to the Internet and checks if there is
     * a newer version of the application online
     */

    void checkForUpdates(void);

private slots:
    void showLatestVersion(void);
    void onCheckingFinished(void);
    void showUpdateAvailable(void);

protected:
    bool event(QEvent *_event);

private:
    Window *m_window;
    QSettings *m_settings;
    QSimpleUpdater *m_updater;

    bool m_show_all_updater_messages;
};

#endif
