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

#include <QUrl>
#include <QFile>
#include <QDateTime>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopServices>

#include <Qsci/qsciprinter.h>

#include "window.h"
#include "menubar.h"
#include "toolbar.h"
#include "platform.h"
#include "statusbar.h"
#include "searchdialog.h"
#include "assembly_info.h"

#define CURRENT_YEAR QDateTime::currentDateTime().toString("yyyy")

Window::Window (void)
{
    // Allow other instances of Window
    // to communicate with each other
    setObjectName ("window");

    // The application crashes on Mac OS X if
    // WA_DeleteOnClose attribute is defined
    if (!MAC_OS_X)
        setAttribute (Qt::WA_DeleteOnClose);

    // Create the core components
    m_editor = new Editor (this);
    m_toolbar = new ToolBar (this);
    m_statusbar = new StatusBar (this);
    m_search_dialog = new SearchDialog (this);
    m_menu = new MenuBar (this);
    m_settings = new QSettings (APP_COMPANY, APP_NAME);

    // Change the title of the window when a new file is loaded
    connect (editor(), SIGNAL (updateTitle()), this, SLOT (updateTitle()));

    // Append a "*" to the document name when the file is modified
    connect (editor(), SIGNAL (textChanged()), this, SLOT (updateTitle()));

    // Sync the settings across the editor and the window
    connect (editor(), SIGNAL (settingsChanged()), this, SIGNAL (settingsChanged()));
    connect (this,     SIGNAL (updateSettings()), editor(), SLOT (updateSettings()));

    // Configure all widgets
    updateTitle();
    updateSettings();
    setCentralWidget (editor());

    // Set window geometry
    setMinimumSize (420, 420);
    resize (m_settings->value ("size", QSize (640, 420)).toSize());
    move (m_settings->value ("position", QPoint (200, 200)).toPoint());
    m_settings->value ("maximized", false).toBool() ? showMaximized() : showNormal();
}

Editor *Window::editor (void) const
{
    return m_editor;
}

ToolBar *Window::toolbar (void) const
{
    return m_toolbar;
}

void Window::closeEvent (QCloseEvent *event)
{
    saveWindowState();

    // Save the document before closing
    m_editor->maybeSave() ? event->accept() : event->ignore();
}

void Window::openFile (const QString &file_name)
{
    // Open the file in the same window
    if (m_editor->titleIsShit() && !m_editor->isModified())
        m_editor->readFile (file_name);

    // Open the file in another window
    else
    {
        Window *_window = new Window();
        configureWindow (_window);
        _window->editor()->readFile (file_name);
    }
}

void Window::newFile (void)
{
    // Create a new window  and configure it
    Window *_window = new Window();
    configureWindow (_window);
}

void Window::open (void)
{
    QStringList _files = QFileDialog::getOpenFileNames (this, tr ("Open"), QDir::homePath());

    // Open each file separately
    for (int i = 0; i < _files.count(); ++i)
        if (!_files.at (i).isEmpty())
            openFile (_files.at (i));
}

void Window::setReadOnly (bool ro)
{
    m_editor->setReadOnly (ro);
    m_toolbar->setReadOnly (ro);

    emit readOnlyChanged (ro);
}

void Window::setWordWrap (bool ww)
{
    m_settings->setValue ("wordwrap-enabled", ww);
    syncSettings();
}

void Window::setToolbarText (bool tt)
{
    m_settings->setValue ("toolbar-text", tt);
    syncSettings();
}

void Window::setToolbarEnabled (bool tb)
{
    m_settings->setValue ("toolbar-enabled", tb);
    syncSettings();
}

void Window::setStatusBarEnabled (bool sb)
{
    m_settings->setValue ("statusbar-enabled", sb);
    syncSettings();
}

void Window::setHCLineEnabled (bool hc)
{
    m_settings->setValue ("hc-line-enabled", hc);
    syncSettings();
}

void Window::setUseLargeIcons (bool li)
{
    m_settings->setValue ("large-icons", li);
    syncSettings();
}

void Window::setLineNumbersEnabled (bool ln)
{
    m_settings->setValue ("line-numbers-enabled", ln);
    syncSettings();
}

void Window::setColorscheme (const QString &colorscheme)
{
    m_settings->setValue ("color-scheme", colorscheme);
    syncSettings();
}

void Window::showFindReplaceDialog (void)
{
    m_search_dialog->show();
}

void Window::setIconTheme (const QString &theme)
{
    m_settings->setValue ("icon-theme", theme);
    syncSettings();
}

void Window::aboutThunderpad (void)
{
    // Show a nice about dialog with application information
    QString _message = QString ("<h%1>%2 %3</h%1>").arg (MAC_OS_X ? "3" : "2", APP_NAME, APP_VERSION) +
                       QString ("<span style='font-weight:normal;'><font size=%1>").arg (MAC_OS_X ? "2" : "3") +
                       "<p>" + tr ("Built on %1 at %2").arg (__DATE__, __TIME__) + "</p>" +
                       "<p>" + tr ("Copyright &copy; 2013-%1 %2").arg (CURRENT_YEAR, APP_COMPANY) + "</p>" +
                       "<p>" + tr (GNU_WARRANTY_WARNING) + "</p></font></span>";

    QMessageBox::about (this, tr ("About %1").arg (APP_NAME), _message);
}

void Window::license (void)
{
    QDesktopServices::openUrl (QUrl (LICENSE_LINK));
}

void Window::donate (void)
{
    QDesktopServices::openUrl (QUrl (DONATE_LINK));
}

void Window::viewHelp (void)
{
    QDesktopServices::openUrl (QUrl (HELP_LINK));
}

void Window::sendFeedback (void)
{
    QDesktopServices::openUrl (QUrl (FEED_BACK_LINK));
}

void Window::reportBug (void)
{
    QDesktopServices::openUrl (QUrl (REPORT_ISSUES_LINK));
}

void Window::makeContribution (void)
{
    QDesktopServices::openUrl (QUrl (CONTRIBUTE_LINK));
}

void Window::officialWebsite (void)
{
    QDesktopServices::openUrl (QUrl (WEBSITE_LINK));
}

void Window::updateTitle (void)
{
    // Use "Untitled" while editing new documents
    QString _title = editor()->documentTitle().isEmpty() ?
                     tr ("Untitled") :
                     shortFileName (editor()->documentTitle());

    // Add a "*" if the document was modifed
    QString _star = editor()->isModified() ?  "* - " : " - ";
    setWindowTitle (_title + _star + APP_NAME);

    // Configure the behavior of the 'smart' save actions
    bool _save_enabled = ! (!m_editor->titleIsShit() &&
                            !m_editor->isModified());

    m_menu->setSaveEnabled (_save_enabled);
    m_toolbar->setSaveEnabled (_save_enabled);
}

void Window::syncSettings (void)
{
    emit updateSettings();
    emit settingsChanged();
}

void Window::saveWindowState (void)
{
    m_settings->setValue ("maximized", isMaximized());

    // Only save the window size and position if it isn't maximized
    if (!isMaximized())
    {
        m_settings->setValue ("size", size());
        m_settings->setValue ("position", pos());
    }
}

void Window::configureWindow (Window *window)
{
    Q_ASSERT (window != NULL);

    // Allow the other window to ask the application to check for updates
    connect (window, SIGNAL (checkForUpdates()), this, SIGNAL (checkForUpdates()));

    // Sync settings across windows
    foreach (QWidget *widget, QApplication::topLevelWidgets())
    {
        if (widget->objectName() == objectName())
        {
            connect (widget, SIGNAL (settingsChanged()), window, SIGNAL (updateSettings()));
            connect (window, SIGNAL (settingsChanged()), widget, SIGNAL (updateSettings()));
        }
    }

    // Show the window normally if the current window is maximized
    if (isMaximized())
        window->showNormal();

    // Resize the window and position it to match the current window
    else
    {
        window->resize (size());
        window->move (window->x() + 30, window->y() + 30);
        m_settings->setValue ("position", QPoint (window->x(), window->y()));
    }
}

QString Window::shortFileName (const QString &file)
{
    Q_ASSERT (!file.isEmpty());
    return QFileInfo (file).fileName();
}
