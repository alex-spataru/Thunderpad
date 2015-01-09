//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2015 Alex Spataru <alex_spataru@outlook.com>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301
//  USA
//

#include "app.h"
#define QUIT_ON_LAST_WINDOW true

Application::Application (int &argc, char **argv) : QApplication (argc, argv),
    m_show_all_updater_messages (false)
{

    // Set application info
    setApplicationName (APP_NAME);
    setOrganizationName (APP_COMPANY);
    setApplicationVersion (APP_VERSION);
    setWindowIcon (QIcon (":/icons/logo.png"));
    setQuitOnLastWindowClosed (QUIT_ON_LAST_WINDOW);

    // Initialize shared components
    m_updater = new QSimpleUpdater();
    m_settings = new QSettings (APP_COMPANY, APP_NAME);

    // Read system arguments
    QString arguments;

    if (argc != 1)
        {
        for (int i = 0; i < argc; i++)
            arguments = argv[i];
        }

    // We are the first instance of Thunderpad, create a new window and
    // lock the running status so that we are the only instance allowed to run
    if (!m_settings->value ("running", false).toBool())
        {
        runNormally (arguments);
        return;
        }

    // There's already a running instance of Thunderpad, send data and quit
    else
        {
        m_first_instance = false;
        int instances_closed = m_settings->value ("instances-closed", 0).toInt();

        // Write instance information
        m_settings->setValue ("another-instance-was-executed", true);
        m_settings->setValue ("instances-closed", instances_closed + 1);
        m_settings->setValue ("arguments-from-other-instances", arguments);

        // Reset the settings if Thunderpad crashed before
        if (instances_closed > 0)
            {
            m_settings->setValue ("running", false);
            m_settings->setValue ("instances-closed", 0);
            m_settings->setValue ("another-instance-was-executed", false);

            runNormally (arguments);
            }
        }
}

bool Application::isFirstInstance (void) const
{
    return m_first_instance;
}

void Application::checkForUpdates (void)
{
    m_updater->checkForUpdates();
    m_show_all_updater_messages = true;
}

void Application::setupUpdater (void)
{
    QString download_package_url;
    QString base_repo_url = "https://raw.githubusercontent.com/"
                            "alex-97/thunderpad/updater/";

    // Decide which file to download
#if MAC_OS_X
    download_package_url = base_repo_url + "files/thunderpad-latest.dmg";
#elif WINDOWS
    download_package_url = base_repo_url + "files/thunderpad-latest.exe";
#else
    download_package_url = base_repo_url + "files/thunderpad-latest.tar.gz";
#endif

    // Specify updater details
    m_updater->setDownloadUrl (download_package_url);
    m_updater->setApplicationVersion (applicationVersion());
    m_updater->setReferenceUrl (base_repo_url + "latest.txt");
    m_updater->setChangelogUrl (base_repo_url + "changelog.txt");

    // Show message box when new update is found
    connect (m_updater, SIGNAL (checkingFinished()), this, SLOT (onCheckingFinished()));

    // Check for updates automatically
    if (m_settings->value ("check-for-updates", true).toBool())
        m_updater->checkForUpdates();
}

void Application::onAboutToQuit (void)
{

    // Unlock the running state so that other instances can be launched
    if (!m_settings->value ("another-instance-was-executed", false).toBool())
        m_settings->setValue ("running", false);

    // Quit the application
    exit (0);
}

void Application::showLatestVersion (void)
{
    QMessageBox _message;
    _message.setStandardButtons (QMessageBox::Ok);
    _message.setIconPixmap (QPixmap (":/icons/logo.png"));
    _message.setInformativeText (
        tr ("The latest release of Thunderpad is version %1")
        .arg (qApp->applicationVersion()));
    _message.setText ("<b>" + tr ("Congratulations! You are running the latest "
                                  "version of Thunderpad!") +
                      "</b>");

    _message.exec();
}

void Application::showUpdateAvailable (void)
{
    QMessageBox _message;
    _message.setDetailedText (m_updater->changeLog());
    _message.setIconPixmap (QPixmap (":/icons/logo.png"));
    _message.setStandardButtons (QMessageBox::Yes | QMessageBox::No);
    _message.setText ("<b>" + tr ("There's a new version of Thunderpad!") + " (" +
                      m_updater->latestVersion() + ")</b>");
    _message.setInformativeText (
        tr ("Do you want to download the newest version?"));

    if (_message.exec() == QMessageBox::Yes)
        m_updater->downloadLatestVersion();
}

void Application::checkForOtherInstances (void)
{
    // The system tried to open a new instance of Thunderpad, get the instance
    // information and load it in a new window
    if (m_settings->value ("another-instance-was-executed", false).toBool())
        {
        Window *_window = new Window();
        _window->openFile (m_settings->value ("arguments-from-other-instances", "").toString());
        m_window->configureWindow (_window);

        // Reset instance settings
        m_settings->setValue ("instances-closed", 0);
        m_settings->setValue ("arguments-from-other-instances", "");
        m_settings->setValue ("another-instance-was-executed", false);
        }

    // Continue instance refresh loop
    m_instance_refresh_timer->start (500);
}

void Application::runNormally (const QString &arguments)
{
    m_window = new Window();
    m_first_instance = true;
    m_window->openFile (arguments);
    m_instance_refresh_timer = new QTimer (this);

    connect (this, SIGNAL (lastWindowClosed()), this, SLOT (onAboutToQuit()));
    connect (m_window, SIGNAL (checkForUpdates()), this, SLOT (checkForUpdates()));
    connect (m_instance_refresh_timer, SIGNAL (timeout()), this, SLOT (checkForOtherInstances()));

    m_instance_refresh_timer->start (10);
    m_settings->setValue ("running", true);

    setupUpdater();
    showWelcomeMessages();
}

void Application::onCheckingFinished (void)
{
    if (m_updater->newerVersionAvailable())
        showUpdateAvailable();

    else if (m_show_all_updater_messages)
        {
        showLatestVersion();
        m_show_all_updater_messages = false;
        }
}

void Application::showWelcomeMessages (void)
{
    QMessageBox _message;
    _message.setWindowModality (Qt::WindowModal);
    _message.setIconPixmap (QPixmap (":/icons/logo.png"));

    // Welcome the user to Thunderpad
    if (m_settings->value ("first-launch", true).toBool())
        {
        _message.setStandardButtons (QMessageBox::Close);
        _message.setText ("<b>" + tr ("Thank you for downloading Thunderpad!") +
                          "</b>           ");
        _message.setInformativeText (
            tr ("If you find this program useful and would like to help "
                "contribute to future development, please consider "
                "a small donation. You can  use the Donate item in the "
                "Help menu to send your much needed assistance via BitCoins.\n\n"
                "Please share Thunderpad with your friends and colleagues, "
                "and feel free to send me feedback!"));
        _message.exec();

        m_settings->setValue ("first-launch", false);
        m_settings->setValue ("second-launch", true);
        }

    // Ask the user to check for updates automatically
    else if (m_settings->value ("second-launch", false).toBool())
        {
        _message.setDefaultButton (QMessageBox::Yes);
        _message.setStandardButtons (QMessageBox::Yes | QMessageBox::No);
        _message.setText (
            "<b>" + tr ("Do you want to check for updates automatically?") + "</b>");
        _message.setInformativeText (tr ("You can always check for updates from the "
                                         "Help menu"));

        m_settings->setValue ("second-launch", false);
        m_settings->setValue ("check-for-updates", _message.exec() == QMessageBox::Yes);
        }
}

bool Application::event (QEvent *_event)
{

    // System requested us to open a file
    if (_event->type() == QEvent::FileOpen)
        m_window->openFile (static_cast<QFileOpenEvent *> (_event)->file());

    // System requested us to do another thing
    else
        return QApplication::event (_event);

    return true;
}
