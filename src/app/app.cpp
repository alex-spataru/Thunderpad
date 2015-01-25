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

Application::Application (int &argc, char **argv) : QApplication (argc, argv)
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
    
    // Open the requested file (or create blank file)
    m_window->openFile (arguments);

    // Enable manual check for updates
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
    m_updater->setSilent(false);
    m_updater->setShowNewestVersionMessage(true);
    m_updater->checkForUpdates();
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
    m_updater->setSilent(true);
    m_updater->setDownloadUrl (download_package_url);
    m_updater->setApplicationVersion (applicationVersion());
    m_updater->setReferenceUrl (base_repo_url + "latest.txt");
    m_updater->setChangelogUrl (base_repo_url + "changelog.txt");
    
    // Check for updates automatically
    if (m_settings->value ("check-for-updates", SETTINGS_AUTO_CHECK_UPDATES).toBool())
        m_updater->checkForUpdates();
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
