//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#include "app.h"

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv),
      m_show_all_updater_messages(false) {

    setApplicationName(APP_NAME);
    setOrganizationName(APP_COMPANY);
    setApplicationVersion(APP_VERSION);

    m_window = new Window();
    m_updater = new QSimpleUpdater();
    m_settings = new QSettings(APP_COMPANY, APP_NAME);

    if (argc != 1) {
        QString _args;

        for (int i = 0; i < argc; i++)
            _args = argv[i];

        m_window->openFile(_args);
    }

    connect(m_window, SIGNAL(checkForUpdates()), this, SLOT(checkForUpdates()));
    connect(m_updater, SIGNAL(checkingFinished()), this,
            SLOT(onCheckingFinished()));

    QString download_package_url;
    QString base_repo_url = "https://raw.githubusercontent.com/"
                            "alex-97/thunderpad/updater/";

#if MAC_OS_X
    download_package_url = base_repo_url + "files/thunderpad-latest.dmg";
#elif WINDOWS
    download_package_url = base_repo_url + "files/thunderpad-latest.exe";
#else
    download_package_url = base_repo_url + "files/thunderpad-latest.tar.gz";
#endif

    m_updater->setDownloadUrl(download_package_url);
    m_updater->setApplicationVersion(applicationVersion());
    m_updater->setReferenceUrl(base_repo_url + "latest.txt");
    m_updater->setChangelogUrl(base_repo_url + "changelog.txt");

    QMessageBox _message;
    _message.setParent(m_window);
    _message.setWindowTitle(" ");
    _message.setWindowModality(Qt::WindowModal);
    _message.setWindowIcon(QIcon(":/icons/dummy.png"));
    _message.setIconPixmap(QPixmap(":/icons/logo.png"));

    if (m_settings->value("first-launch", true).toBool()) {
        _message.setStandardButtons(QMessageBox::Close);
        _message.setText("<b>" + tr("Thank you for downloading Thunderpad!") +
                         "</b>           ");
        _message.setInformativeText(
                    tr("If you find this program useful and would like to help "
                       "contribute to future development, please consider "
                       "a small donation. You can  use the Donate item in the "
                       "Help menu to send your much needed assistance via BitCoins.\n\n"
                       "Please share Thunderpad with your friends and colleagues, "
                       "and feel free to send me feedback!"));
        _message.exec();

        m_settings->setValue("first-launch", false);
        m_settings->setValue("second-launch", true);

    }

    else if (m_settings->value("second-launch", false).toBool()) {
        _message.setDefaultButton(QMessageBox::Yes);
        _message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        _message.setText(
                    "<b>" + tr("Do you want to check for updates automatically?") + "</b>");
        _message.setInformativeText(tr("You can always check for updates from the "
                                       "Help menu"));

        if (_message.exec() == QMessageBox::Yes)
            m_settings->setValue("check-for-updates", true);

        else
            m_settings->setValue("check-for-updates", false);

        m_settings->setValue("second-launch", false);
    }

    if (m_settings->value("check-for-updates", true).toBool()) {
        qDebug() << this << "Checking for updates...";
        m_updater->checkForUpdates();
    }
}

int Application::showInitError(void) {
    qCritical() << this << "There's a running instance of" << APP_NAME;
    QMessageBox::warning(
                NULL, tr("Application error"),
                tr("There's already a running instance of %1").arg(applicationName()));

    return -1;
}

void Application::checkForUpdates(void) {
    qDebug() << this << "Checking for updates...";
    m_updater->checkForUpdates();
    m_show_all_updater_messages = true;
}

void Application::showLatestVersion(void) {
    qDebug() << this << "You have the latest version installed!";

    QMessageBox _message;
    _message.setStandardButtons(QMessageBox::Ok);
    _message.setWindowIcon(QIcon(":/icons/dummy.png"));
    _message.setIconPixmap(QPixmap(":/icons/logo.png"));
    _message.setWindowTitle(tr("No updates available"));
    _message.setInformativeText(
                tr("The latest release of Thunderpad is version %1")
                .arg(qApp->applicationVersion()));
    _message.setText("<b>" + tr("Congratulations! You are running the latest "
                                "version of Thunderpad!") +
                     "</b>");

    _message.exec();
}

void Application::showUpdateAvailable(void) {
    qDebug() << this << "An application update was found online!";

    QMessageBox _message;
    _message.setWindowTitle(" ");
    _message.setDetailedText(m_updater->changeLog());
    _message.setWindowIcon(QIcon(":/icons/dummy.png"));
    _message.setIconPixmap(QPixmap(":/icons/logo.png"));
    _message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    _message.setText("<b>" + tr("There's a new version of Thunderpad!") + " (" +
                     m_updater->latestVersion() + ")</b>");
    _message.setInformativeText(
                tr("Do you want to download the newest version?"));

    if (_message.exec() == QMessageBox::Yes)
        m_updater->downloadLatestVersion();
}

void Application::onCheckingFinished(void) {
    if (m_updater->newerVersionAvailable())
        showUpdateAvailable();

    else if (m_show_all_updater_messages) {
        showLatestVersion();
        m_show_all_updater_messages = false;
    }
}

bool Application::event(QEvent *_event) {
    qDebug() << this << "System event:" << _event->type();

    if (_event->type() == QEvent::FileOpen)
        m_window->openFile(static_cast<QFileOpenEvent *>(_event)->file());

    else
        return QApplication::event(_event);

    return true;
}
