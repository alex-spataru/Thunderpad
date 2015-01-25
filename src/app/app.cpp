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
#include "defaults.h"
#include "platform.h"
#include "assembly_info.h"

/*!
 * \class Application
 * \brief Creates a bridge between the operating system and the main window
 *
 * The \c Application class is in charge of creating the main window
 * and communicating with the operating system in order to:
 *
 * \list
 * \o Implement a single instance application
 * \o Load files requested by the operating system (ex: from the file manager)
 * \endlist
 */

/*!
 * \internal
 * Initializes the application and sets the application information
 */

Application::Application (int &argc, char **argv) : QtSingleApplication (argc, argv)
{
    setApplicationName (APP_NAME);
    setOrganizationName (APP_COMPANY);
    setApplicationVersion (APP_VERSION);
    setWindowIcon (QIcon (":/images/others/logo.png"));
}

/*!
 * Creates and configures the main window, configures the auto-updater
 * and shows welcome messages.
 */

int Application::start (const QString &arguments)
{
    m_window = new Window();
    m_updater = new QSimpleUpdater();
    m_settings = new QSettings (APP_COMPANY, APP_NAME);

    m_window->openFile (arguments);

    connect (m_window, SIGNAL (checkForUpdates()), this, SLOT (checkForUpdates()));
    connect (this, SIGNAL (messageReceived (QString)), this, SLOT (onMessageReceived (QString)));

    QTimer *timer = new QTimer (this);
    timer->singleShot (250, this, SLOT (setupUpdater()));

    showWelcomeMessages();

    return exec();
}

/*!
 * Changes the configuration of the updater system to show all
 * messages and checks for updates
 */

void Application::checkForUpdates (void)
{
    m_updater->setSilent (false);
    m_updater->setShowNewestVersionMessage (true);
    m_updater->checkForUpdates();
}

/*!
 * Configures the auto-updater system and checks for updates automatically
 */

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
    m_updater->setSilent (true);
    m_updater->setDownloadUrl (download_package_url);
    m_updater->setApplicationVersion (applicationVersion());
    m_updater->setReferenceUrl (base_repo_url + "latest.txt");
    m_updater->setChangelogUrl (base_repo_url + "changelog.txt");

    // Check for updates automatically
    if (m_settings->value ("check-for-updates", SETTINGS_AUTO_CHECK_UPDATES).toBool())
        m_updater->checkForUpdates();
}

/*!
 * \list
 * \o Welcomes the user to Thunderpad during first launch
 * \o Prompts the user to allow the software to check for updates
 * automatically during second launch
 * \endlist
 */

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

/*!
 * Gets data from other instances of the application and
 * acts accordingly (ex: open a file or create a new one)
 */

void Application::onMessageReceived (const QString &msg)
{
    if (!msg.isEmpty())
        m_window->openFile (msg);

    else
        m_window->newFile();
}

/*!
 * Allows to open files specified by an operating system event
 * (ex: when you open a file from the file manager)
 */

bool Application::event (QEvent *_event)
{
    if (_event->type() == QEvent::FileOpen)
        m_window->openFile (static_cast<QFileOpenEvent *> (_event)->file());

    else
        return QApplication::event (_event);

    return true;
}
