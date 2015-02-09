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
#include <QSettings>
#include <QDateTime>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QDesktopServices>

#include <QMoveEvent>
#include <QCloseEvent>

#include <Qsci/qsciprinter.h>

#include "editor.h"
#include "window.h"
#include "menubar.h"
#include "toolbar.h"
#include "platform.h"
#include "statusbar.h"
#include "searchdialog.h"

#define CURRENT_YEAR QDateTime::currentDateTime().toString("yyyy")

#define GNU_WARRANTY_WARNING "The program is provided AS IS with NO WARRANTY \
OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, \
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE."

#define HELP_LINK "http://thunderpad.sf.net/support"
#define DONATE_LINK "http://thunderpad.sf.net/donate"
#define FEED_BACK_LINK "mailto:alex_spataru@outlook.com"
#define WEBSITE_LINK "http://thunderpad.sourceforge.net"
#define LICENSE_LINK "http://www.gnu.org/copyleft/gpl.html"
#define CONTRIBUTE_LINK "http://thunderpad.sf.net/contribute"
#define REPORT_ISSUES_LINK "http://github.com/alex-97/thunderpad/issues"

/*!
 * \class Window
 *
 * The \c Window class is used to display and configure each element
 * of Thunderpad, such as the \c Editor widget.
 *
 * The \c Window is in charge of connecting and configuring everything together,
 * such as the actions of the menubar with the functions of the text editor.
 * Finally, the \c Window allows the \c Application class to create a new window
 * and open a file using the configureWindow() function.
 */

Window::Window (const QString &file) {
    //
    // Allow other instances of Window
    // to communicate with each other
    //
    setObjectName ("window");

    //
    // Create the core components
    //
    m_editor = new Editor();
    m_toolbar = new ToolBar (this);
    m_statusbar = new StatusBar (this);
    m_search_dialog = new SearchDialog (this);
    m_menu = new MenuBar (this);
    m_settings = new QSettings (APP_COMPANY, APP_NAME);

    //
    // Change the title of the window when a new file is loaded
    //
    connect (editor(), SIGNAL (updateTitle()), this, SLOT (updateTitle()));

    //
    // Append a "*" to the document name when the file is modified
    //
    connect (editor(), SIGNAL (textChanged()), this, SLOT (updateTitle()));

    //
    // Sync the settings across the editor and the window
    //
    connect (editor(), SIGNAL (settingsChanged()), this, SIGNAL (settingsChanged()));
    connect (this,     SIGNAL (updateSettings()), editor(), SLOT (updateSettings()));

    //
    // Configure all widgets
    //
    updateTitle();
    updateSettings();
    setCentralWidget (editor());

    //
    // Set window geometry
    //
    setMinimumSize (420, 420);
    resize (settings()->value ("size", QSize (640, 420)).toSize());
    move (settings()->value ("position", QPoint (200, 200)).toPoint());
    settings()->value ("maximized", false).toBool() ? showMaximized() : showNormal();

    //
    // Read file
    //
    if (!file.isEmpty())
        editor()->readFile (file);
}

Window::~Window (void) {
    delete m_menu;
    delete m_editor;
    delete m_toolbar;
    delete m_settings;
    delete m_statusbar;
    delete m_search_dialog;
}

Editor *Window::editor (void) const {
    return m_editor;
}

ToolBar *Window::toolbar (void) const {
    return m_toolbar;
}

MenuBar *Window::menubar (void) const {
    return m_menu;
}

QSettings *Window::settings (void) const {
    return m_settings;
}

SearchDialog *Window::searchDialog (void) const {
    return m_search_dialog;
}

void Window::moveEvent (QMoveEvent *event) {
    saveWindowState();
    event->accept();
}

void Window::closeEvent (QCloseEvent *event) {
    editor()->maybeSave() ? event->accept() : event->ignore();
}

void Window::resizeEvent (QResizeEvent *event) {
    saveWindowState();
    event->accept();
}

void Window::openFile (const QString &file_name) {
    //
    // Open the file in the same window
    //
    if (editor()->titleIsShit() && !editor()->isModified())
        editor()->readFile (file_name);

    //
    // Open the file in another window
    //
    else
        configureWindow (new Window (file_name));
}

void Window::newFile (void) {
    configureWindow (new Window (""));
}

void Window::open (void) {
    QStringList _files = QFileDialog::getOpenFileNames (this, tr ("Open"), QDir::homePath());

    //
    // Open each file separately
    //
    for (int i = 0; i < _files.count(); ++i)
        if (!_files.at (i).isEmpty())
            openFile (_files.at (i));
}

void Window::setReadOnly (bool ro) {
    editor()->setReadOnly (ro);
    toolbar()->setReadOnly (ro);

    emit readOnlyChanged (ro);
}

void Window::setWordWrap (bool ww) {
    settings()->setValue ("wordwrap-enabled", ww);
    syncSettings();
}

void Window::setToolbarText (bool tt) {
    settings()->setValue ("toolbar-text", tt);
    syncSettings();
}

void Window::setToolbarEnabled (bool tb) {
    settings()->setValue ("toolbar-enabled", tb);
    syncSettings();
}

void Window::setStatusBarEnabled (bool sb) {
    settings()->setValue ("statusbar-enabled", sb);
    syncSettings();
}

void Window::setHCLineEnabled (bool hc) {
    settings()->setValue ("hc-line-enabled", hc);
    syncSettings();
}

void Window::setUseLargeIcons (bool li) {
    settings()->setValue ("large-icons", li);
    syncSettings();
}

void Window::setLineNumbersEnabled (bool ln) {
    settings()->setValue ("line-numbers-enabled", ln);
    syncSettings();
}

void Window::setColorscheme (const QString &colorscheme) {
    settings()->setValue ("color-scheme", colorscheme);
    syncSettings();
}

void Window::showFindReplaceDialog (void) {
    searchDialog()->show();
}

void Window::setIconTheme (const QString &theme) {
    settings()->setValue ("icon-theme", theme);
    syncSettings();
}

void Window::aboutThunderpad (void) {
    //
    // Show a nice about dialog with application information
    //
    QString _message = QString ("<h%1>%2 %3</h%1>").arg (MAC_OS_X ? "3" : "2", APP_NAME, APP_VERSION) +
                       QString ("<span style='font-weight:normal;'><font size=%1>").arg (MAC_OS_X ? "2" : "3") +
                       "<p>" + tr ("Built on %1 at %2").arg (__DATE__, __TIME__) + "</p>" +
                       "<p>" + tr ("Copyright &copy; 2013-%1 %2").arg (CURRENT_YEAR, APP_COMPANY) + "</p>" +
                       "<p>" + tr (GNU_WARRANTY_WARNING) + "</p></font></span>";

    QMessageBox::about (this, tr ("About %1").arg (APP_NAME), _message);
}

void Window::license (void) {
    QDesktopServices::openUrl (QUrl (LICENSE_LINK));
}

void Window::donate (void) {
    QDesktopServices::openUrl (QUrl (DONATE_LINK));
}

void Window::viewHelp (void) {
    QDesktopServices::openUrl (QUrl (HELP_LINK));
}

void Window::sendFeedback (void) {
    QDesktopServices::openUrl (QUrl (FEED_BACK_LINK));
}

void Window::reportBug (void) {
    QDesktopServices::openUrl (QUrl (REPORT_ISSUES_LINK));
}

void Window::makeContribution (void) {
    QDesktopServices::openUrl (QUrl (CONTRIBUTE_LINK));
}

void Window::officialWebsite (void) {
    QDesktopServices::openUrl (QUrl (WEBSITE_LINK));
}

void Window::updateTitle (void) {
    //
    // Use "Untitled" while editing new documents
    //
    QString _title = editor()->documentTitle().isEmpty() ?
                     tr ("Untitled") :
                     shortFileName (editor()->documentTitle());

    //
    // Add a "*" if the document was modifed
    //
    QString _star = editor()->isModified() ?  "* - " : " - ";
    setWindowTitle (_title + _star + APP_NAME);

    //
    // Configure the behavior of the 'smart' save actions
    //
    bool _save_enabled = ! (!editor()->titleIsShit() &&
                            !editor()->isModified());

    menubar()->setSaveEnabled (_save_enabled);
    toolbar()->setSaveEnabled (_save_enabled);
}

void Window::syncSettings (void) {
    emit updateSettings();
    emit settingsChanged();
}

void Window::saveWindowState (void) {
    settings()->setValue ("maximized", isMaximized());

    //
    // Mac OS X does not register a window as maximized,
    // so we take into account the window size instead
    //
    bool _mac_os_maximized = false;

    if (MAC_OS_X) {
        int d_width = QApplication::desktop()->width() - 100;
        int d_height = QApplication::desktop()->height() - 100;

        if (width() >= d_width || height() >= d_height)
            _mac_os_maximized = true;
    }

    //
    // Only save the window size and position if it isn't maximized
    //
    if (!isMaximized() && !_mac_os_maximized) {
        settings()->setValue ("size", size());
        settings()->setValue ("position", pos());
    }
}

void Window::configureWindow (Window *window) {
    Q_ASSERT (window != NULL);

    //
    // Allow the other window to ask the application to check for updates
    //
    connect (window, SIGNAL (checkForUpdates()), this, SIGNAL (checkForUpdates()));

    //
    // Sync settings across windows
    //
    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        if (widget->objectName() == objectName()) {
            connect (widget, SIGNAL (settingsChanged()), window, SIGNAL (updateSettings()));
            connect (window, SIGNAL (settingsChanged()), widget, SIGNAL (updateSettings()));
        }
    }

    //
    // Show the window normally if the current window is maximized
    //
    if (isMaximized())
        window->showNormal();

    //
    // Resize the window and position it to match the current window
    //
    else {
        int _x = window->x() + 45;
        int _y = window->y() + 75;
        QDesktopWidget *desktop = QApplication::desktop();

        //
        // Resize the window to be the same as its creator
        //
        window->resize (size());

        //
        // Make sure that the window fits inside
        // the desktop
        //
        {
            if (_x + width() >= desktop->width() - 50)
                _x = 45;

            if (_y + height() >= desktop->height() - 50)
                _y = 75;
        }

        //
        // Reposition the window
        //
        window->move (_x, _y);
    }
}

QString Window::shortFileName (const QString &file) {
    Q_ASSERT (!file.isEmpty());
    return QFileInfo (file).fileName();
}
