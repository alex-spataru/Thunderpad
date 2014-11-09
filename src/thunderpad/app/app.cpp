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
    m_updater = new QSimpleUpdater();

    if (argc != 1)
    {
        QString _args;

        for (int i = 0; i < argc; i++) _args = argv[i];

        m_window->openFile (_args);
    }

    connect (m_window, SIGNAL (checkForUpdates()),
             this,     SLOT   (checkForUpdates()));
    connect (m_updater, SIGNAL (checkingFinished()),
             this,      SLOT   (onCheckingFinished()));

    QString _download_url;
    QString _url_base = "https://raw.githubusercontent.com/alex-97/thunderpad/updater/files/";

#if defined(Q_OS_MAC)
    _download_url = _url_base + "thunderpad-latest.dmg";
#elif defined(Q_OS_WIN32)
    _download_url = _url_base + "thunderpad-latest.exe";
#elif defined(Q_OS_ANDROID)
    _download_url = _url_base + "thunderpad-latest.apk";
#elif defined(Q_OS_LINUX)
    _download_url = _url_base + "thunderpad-latest.tar.gz";
#endif

    m_updater->setApplicationVersion (APP_VERSION);
    m_updater->setDownloadUrl  (_download_url);
    m_updater->setReferenceUrl ("https://raw.githubusercontent.com/alex-97/thunderpad/updater/latest.txt");
    m_updater->setChangelogUrl ("https://raw.githubusercontent.com/alex-97/thunderpad/updater/changelog.txt");

    m_updater->checkForUpdates();
}

int Application::showInitError()
{
    QMessageBox::warning (
        NULL, tr ("Application error"),
        tr ("There's already a running instance of %1").arg (applicationName()));

    return -1;
}

void Application::checkForUpdates()
{
    if (m_updater->newerVersionAvailable())
        showUpdateAvailable();

    else
        showLatestVersion();
}

void Application::showLatestVersion()
{
    QMessageBox _message;
    _message.setStandardButtons (QMessageBox::Ok);
    _message.setWindowIcon (QIcon (":/icons/dummy.png"));
    _message.setIconPixmap (QPixmap (":/icons/logo.png"));
    _message.setWindowTitle (tr ("No updates available"));
    _message.setInformativeText (
        tr ("The latest release of Thunderpad is version %1")
        .arg (qApp->applicationVersion()));
    _message.setText ("<b>" + tr ("Congratulations! You are running the latest "
                                  "version of Thunderpad!") +
                      "</b>");

    _message.exec();
}

void Application::showUpdateAvailable()
{
    QMessageBox _message;
    _message.setDetailedText (m_updater->changeLog());
    _message.setWindowIcon (QIcon (":/icons/dummy.png"));
    _message.setIconPixmap (QPixmap (":/icons/logo.png"));
    _message.setWindowTitle (tr ("New updates available"));
    _message.setStandardButtons (QMessageBox::Yes | QMessageBox::No);
    _message.setText ("<b>" + tr ("There's a new version of Thunderpad!") +
                      " (" + m_updater->latestVersion() + ")</b>");
    _message.setInformativeText (
        tr ("Do you want to download the newest version?"));

    if (_message.exec() == QMessageBox::Yes)
        m_updater->downloadLatestVersion();
}

void Application::onCheckingFinished()
{
    if (m_updater->newerVersionAvailable())
        showUpdateAvailable();
}

bool Application::event (QEvent *_event)
{
    if (_event->type() == QEvent::FileOpen)
        m_window->openFile (static_cast<QFileOpenEvent *> (_event)->file());

    else
        return QApplication::event (_event);

    return true;
}
