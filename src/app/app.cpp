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

#include <QTimer>
#include <QMessageBox>

#include "app.h"
#include "platform.h"
#include "assembly_info.h"

Application::Application (int &argc, char **argv) : QApplication (argc, argv),
    m_show_all_updater_messages (false)
{
    setApplicationName (APP_NAME);
    setOrganizationName (APP_COMPANY);
    setApplicationVersion (APP_VERSION);
    setWindowIcon (QIcon (":/images/others/logo.png"));

    QString arguments;

    // Construct a string with the arguments provided by the system
    if (argc != 1)
    {
        for (int i = 0; i < argc; i++)
            arguments = argv[i];
    }

    // Create core components
    m_window = new Window();
    m_updater = new QSimpleUpdater();
    m_settings = new QSettings (APP_COMPANY, APP_NAME);

    // Open the specified file by the system; the window class is very damm smart and
    // can detect whenever we should create a new document or read an existing
    // file. It could also be the bootloader of Skynet someday...
    m_window->openFile (arguments);

    // Check for updates when the user clicks on the "Check for Updates" action
    connect (m_window, SIGNAL (checkForUpdates()), this, SLOT (checkForUpdates()));

    // Setup the updater after an interval...for some unknown reason if we call
    // setupUpdater() directly, the application will delay about 400 ms on Windows
    QTimer *timer = new QTimer (this);
    timer->singleShot (250, this, SLOT (setupUpdater()));

    // Show a welcome message on the first launch of the application,
    // in the second launch ask the user if he/she wants to allow the application
    // to check for updates automatically.
    showWelcomeMessages();
}

void Application::checkForUpdates (void)
{
    m_updater->checkForUpdates();

    // Allow the "you have the newest version installed" message to show up
    m_show_all_updater_messages = true;
}

void Application::setupUpdater (void)
{
    QString download_package_url;
    QString base_repo_url = "https://raw.githubusercontent.com/"
                            "alex-97/thunderpad/updater/";

    // Decide which file should we download based on current OS
#if MAC_OS_X
    download_package_url = base_repo_url + "files/thunderpad-latest.dmg";
#elif WINDOWS
    download_package_url = base_repo_url + "files/thunderpad-latest.exe";
#else
    download_package_url = base_repo_url + "files/thunderpad-latest.tar.gz";
#endif

    // The following code is very obvious...
    m_updater->setDownloadUrl (download_package_url);
    m_updater->setApplicationVersion (applicationVersion());
    m_updater->setReferenceUrl (base_repo_url + "latest.txt");
    m_updater->setChangelogUrl (base_repo_url + "changelog.txt");
    connect (m_updater, SIGNAL (checkingFinished()), this, SLOT (onCheckingFinished()));

    // Check for updates automatically
    if (m_settings->value ("check-for-updates", true).toBool())
        m_updater->checkForUpdates();
}

void Application::showLatestVersion (void)
{
    QMessageBox _message;
    _message.setStandardButtons (QMessageBox::Ok);
    _message.setIconPixmap (QPixmap (":/images/others/logo.png"));

    _message.setInformativeText (
        tr ("The latest release of Thunderpad is version %1")
        .arg (qApp->applicationVersion()));

    _message.setText ("<b>" + tr ("Congratulations! You are running the latest "
                                  "version of Thunderpad!") + "</b>");

    _message.exec();
}

void Application::showUpdateAvailable (void)
{
    QMessageBox _message;
    _message.setDetailedText (m_updater->changeLog());
    _message.setIconPixmap (QPixmap (":/images/others/logo.png"));
    _message.setStandardButtons (QMessageBox::Yes | QMessageBox::No);

    _message.setText ("<b>" + tr ("There's a new version of Thunderpad!") + " (" +
                      m_updater->latestVersion() + ")</b>");

    _message.setInformativeText (
        tr ("Do you want to download the newest version?"));

    // The user wants to download the latest version...
    if (_message.exec() == QMessageBox::Yes)
        m_updater->downloadLatestVersion();
}

void Application::onCheckingFinished (void)
{
    // There's a newer version of the application available to download
    if (m_updater->newerVersionAvailable())
        showUpdateAvailable();

    // Well, the developers are somewhat lazy and haven't released
    // a newer version of the application...should we ask the user
    // to give them some coffee?
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
    _message.setIconPixmap (QPixmap (":/images/others/logo.png"));

    // Its the first launch, welcome the user to the application
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

    // Its the second launch, ask the user if he/she wants to allow the application
    // to check for updates automatically
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
    if (_event->type() == QEvent::FileOpen)
        m_window->openFile (static_cast<QFileOpenEvent *> (_event)->file());

    else
        return QApplication::event (_event);

    return true;
}
