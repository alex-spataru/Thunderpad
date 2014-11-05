//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#include "app.h"

Application::Application (int &argc, char **argv) : QApplication (argc, argv)
{
    setApplicationVersion ("1.0.0");
    setApplicationName ("Thunderpad");
    setOrganizationName ("Alex Spataru");

    m_window = new Window();

    if (argc != 1)
    {
        QString _args;

        for (int i = 0; i < argc; i++) _args = argv[i];

        m_window->openFile (_args);
    }
}

int Application::showInitError()
{
    QMessageBox::warning (
        NULL, tr ("Application error"),
        tr ("There's already a running instance of %1").arg (applicationName()));

    return -1;
}

bool Application::event (QEvent *_event)
{
    if (_event->type() == QEvent::FileOpen)
        m_window->openFile (static_cast<QFileOpenEvent *> (_event)->file());

    else
        return QApplication::event (_event);

    return true;
}
