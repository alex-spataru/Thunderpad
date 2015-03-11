//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2015 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#ifndef APP_H
#define APP_H

#ifdef __APPLE__
extern "C++" {
#endif

class Window;
class QSettings;

#include <qtsingleapplication.h>

class Application : public QtSingleApplication {
        Q_OBJECT

    public:
        Application (int &argc, char **argv);

        int start (const QString &arguments);

    public slots:
        void checkForUpdates (void);

    private slots:
        void setupUpdater (void);
        void showWelcomeMessages (void);
        void onMessageReceived (const QString &msg);

    protected:
        bool event (QEvent *_event);

    private:
        Window *m_window;      
        QSettings *settings (void) const;
};

#endif

#ifdef __APPLE__
}
#endif
