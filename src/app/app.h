//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#ifndef APP_H
#define APP_H

#include <QTimer>
#include <QSettings>
#include <QMessageBox>
#include <QApplication>
#include <QFileOpenEvent>
#include <QSimpleUpdater>

#include "window.h"

class Application : public QApplication {
    Q_OBJECT

public:
    Application(int &argc, char **argv);

public slots:
    void checkForUpdates(void);

private slots:
    void setupUpdater(void);
    void onAboutToQuit(void);
    void showLatestVersion(void);
    void onCheckingFinished(void);
    void showWelcomeMessages(void);
    void showUpdateAvailable(void);
    void checkForOtherInstances(void);

protected:
    bool event(QEvent *_event);

private:
    Window *m_window;
    QSettings *m_settings;
    QSimpleUpdater *m_updater;
    QTimer *m_instance_refresh_timer;

    bool m_show_all_updater_messages;
};

#endif
