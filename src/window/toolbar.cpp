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

#include <QAction>
#include <QSettings>

#include "editor.h"
#include "window.h"
#include "toolbar.h"
#include "platform.h"
#include "defaults.h"

//
// Know where to find the icon themes
//
#define LARGE_ICONS_SIZE "24x24"
#define SMALL_ICONS_SIZE "16x16"
#define ICON_THEMES_PATH ":/images/themes"

//
// Make the toolbar look good on Yosemite
//
#if MAC_OS_X
#include <QSysInfo>
#define LARGE_ICONS_WITH_TEXT QSysInfo::MacintoshVersion >= 11 ? 42 : 48
#define SMALL_ICONS_WITH_TEXT QSysInfo::MacintoshVersion >= 11 ? 32 : 36
#define LARGE_ICONS_WITHOUT_TEXT QSysInfo::MacintoshVersion >= 11 ? 28 : 32
#define SMALL_ICONS_WITHOUT_TEXT QSysInfo::MacintoshVersion >= 11 ? 22 : 24

//
// Define the toolbar size in Windows and Linux
//
#else
#define LARGE_ICONS_WITH_TEXT 56
#define SMALL_ICONS_WITH_TEXT 48
#define LARGE_ICONS_WITHOUT_TEXT 36
#define SMALL_ICONS_WITHOUT_TEXT 32

#endif

/*!
 * \class ToolBar
 *
 * The \c ToolBar class is in charge of creating and configuring
 * a new QToolbarar widget to be used with the \c Window class.
 *
 * This class creates the neccessary actions and connects the signals/slots
 * automatically with the initialize() function.
 */

ToolBar::ToolBar (Window *parent) : QToolBar (parent) {
    setMovable (false);

    //
    // Create the actions
    //
    m_new = new QAction (tr ("New"), this);
    m_open = new QAction (tr ("Open"), this);
    m_save = new QAction (tr ("Save"), this);
    m_print = new QAction (tr ("Print"), this);
    m_undo = new QAction (tr ("Undo"), this);
    m_redo = new QAction (tr ("Redo"), this);
    m_cut = new QAction (tr ("Cut"), this);
    m_copy = new QAction (tr ("Copy"), this);
    m_paste = new QAction (tr ("Paste"), this);
    m_readonly = new QAction (tr ("Read only"), this);
    m_find_replace = new QAction (tr ("Find/Replace"), this);

    //
    // Load the settings
    //
    m_large_icons = settings()->value ("large-icons", SETTINGS_LARGE_ICONS).toBool();
    m_toolbar_text = settings()->value ("toolbar-text", SETTINGS_TOOLBAR_TEXT).toBool();
    setToolbarText (m_toolbar_text);

    //
    // Setup the read only button
    //
    m_readonly->setEnabled (false);
    m_readonly->setVisible (false);
    m_readonly->setCheckable (true);

    //
    // Add the actions to the toolbar
    //
    addAction (m_new);
    addAction (m_open);
    addAction (m_save);
    addSeparator();
    addAction (m_print);
    addSeparator();
    addAction (m_undo);
    addAction (m_redo);
    addSeparator();
    addAction (m_cut);
    addAction (m_copy);
    addAction (m_paste);
    addSeparator();
    addAction (m_find_replace);
    addAction (m_readonly);

    //
    // Add the toolbar to the window
    //
    initialize (parent);
}

/*!
 * Enables/Disables actions used to change the contents of the
 * text editor based on the value of \a {ro}.
 */

void ToolBar::setReadOnly (bool ro) {
    m_readonly->setChecked (ro);
    m_readonly->setEnabled (ro);
    m_readonly->setVisible (ro);

    m_undo->setEnabled (!ro);
    m_redo->setEnabled (!ro);
    m_cut->setEnabled (!ro);
    m_copy->setEnabled (!ro);
    m_paste->setEnabled (!ro);
}

/*!
 * Enables or disables the save button based on the value of
 * \a {enabled}
 */

void ToolBar::setSaveEnabled (bool enabled) {
    m_save->setEnabled (enabled);
}

/*!
 * Hides or shows text under toolbar icons based on the value
 * of \a {enabled}
 */

void ToolBar::setToolbarText (bool enabled) {
    QSize _icon_size;
    short _toolbar_height;

    //
    // Set icon size, use larger values to allow some
    // spacing between the icons.
    //
    _icon_size = m_large_icons ? QSize (32, 32) : QSize (24, 24);

    //
    // Show toolbar text
    //
    if (enabled) {
        _toolbar_height = m_large_icons ? LARGE_ICONS_WITH_TEXT :
                          SMALL_ICONS_WITH_TEXT ;

        setToolButtonStyle (Qt::ToolButtonTextUnderIcon);
    }

    //
    // Hide toolbar text
    //
    else {
        _toolbar_height = m_large_icons ? LARGE_ICONS_WITHOUT_TEXT :
                          SMALL_ICONS_WITHOUT_TEXT ;

        setToolButtonStyle (Qt::ToolButtonIconOnly);
    }

    //
    // Resize the toolbar
    //
    setIconSize (_icon_size);
    setMinimumHeight (_toolbar_height);
    setMaximumHeight (_toolbar_height);

    //
    // Quick and dirty way to redraw the toolbar
    //
    hide();
    show();
}

/*! 
 * Configures the toolbar and adds it to the \a {window}
 */

void ToolBar::initialize (Window *window) {
    Q_ASSERT (window != NULL);

    window->addToolBar (this);
    window->setUnifiedTitleAndToolBarOnMac (true);

    //
    // Connect slots between toolbar and window
    //
    connect (m_new, SIGNAL (triggered()), window, SLOT (newFile()));
    connect (m_open, SIGNAL (triggered()), window, SLOT (open()));
    connect (m_save, SIGNAL (triggered()), window->editor(), SLOT (save()));
    connect (m_print, SIGNAL (triggered()), window->editor(), SLOT (print()));
    connect (m_undo, SIGNAL (triggered()), window->editor(), SLOT (undo()));
    connect (m_redo, SIGNAL (triggered()), window->editor(), SLOT (redo()));
    connect (m_cut, SIGNAL (triggered()), window->editor(), SLOT (cut()));
    connect (m_copy, SIGNAL (triggered()), window->editor(), SLOT (copy()));
    connect (m_paste, SIGNAL (triggered()), window->editor(), SLOT (paste()));
    connect (m_readonly, SIGNAL (triggered (bool)), window, SLOT (setReadOnly (bool)));
    connect (m_find_replace, SIGNAL (triggered()), window,
             SLOT (showFindReplaceDialog()));

    //
    // Change the icon theme when the settings are synced
    //
    connect (window, SIGNAL (updateSettings()), this, SLOT (updateSettings()));
}

void ToolBar::updateSettings (void) {
    bool _new_value = settings()->value ("toolbar-text", SETTINGS_TOOLBAR_TEXT).toBool();
    bool _new_sizes = settings()->value ("large-icons", SETTINGS_LARGE_ICONS).toBool();

    //
    // Resize and redraw the toolbar if neccessary
    //
    if (_new_value != m_toolbar_text || m_large_icons != _new_sizes) {
        m_large_icons = _new_sizes;
        m_toolbar_text = _new_value;

        setToolbarText (_new_value);
    }

    //
    // Hide/show the toolbar
    //
    setVisible (settings()->value ("toolbar-enabled", SETTINGS_TOOLBAR_ENABLED).toBool());
    setEnabled (settings()->value ("toolbar-enabled", SETTINGS_TOOLBAR_ENABLED).toBool());

    //
    // Set icon theme
    //
    update_theme (settings()->value ("icon-theme", SETTINGS_ICON_THEME).toString());
}

void ToolBar::update_theme (const QString &theme) {
    Q_ASSERT (!theme.isEmpty());

    //
    // Load the icon theme in the resources folder
    //
    QString path = QString ("%1/%2/%3/").arg (ICON_THEMES_PATH)
                   .arg (theme)
                   .arg (m_large_icons ?
                         LARGE_ICONS_SIZE :
                         SMALL_ICONS_SIZE);

    //
    // Apply each icon to their respective action
    //
    m_new->setIcon (QIcon (path + "new.png"));
    m_open->setIcon (QIcon (path + "open.png"));
    m_save->setIcon (QIcon (path + "save.png"));
    m_print->setIcon (QIcon (path + "print.png"));
    m_undo->setIcon (QIcon (path + "undo.png"));
    m_redo->setIcon (QIcon (path + "redo.png"));
    m_cut->setIcon (QIcon (path + "cut.png"));
    m_copy->setIcon (QIcon (path + "copy.png"));
    m_paste->setIcon (QIcon (path + "paste.png"));
    m_readonly->setIcon (QIcon (path + "read-only.png"));
    m_find_replace->setIcon (QIcon (path + "search.png"));
}

/*!
 * Allows the class to access the application settings
 */

QSettings *ToolBar::settings (void) const {
    return new QSettings (APP_COMPANY, APP_NAME);
}

