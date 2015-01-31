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

#include "toolbar.h"
#include "platform.h"
#include "defaults.h"

#define LARGE_ICONS_SIZE "24x24"
#define SMALL_ICONS_SIZE "16x16"
#define ICON_THEMES_PATH ":/images/themes"

ToolBar::ToolBar (Window *parent) : QToolBar (parent) {
    setAttribute (Qt::WA_DeleteOnClose);

    // Configure the toolbar
    setMovable (false);

    // Create the actions
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

    // Load the settings
    m_settings = new QSettings (APP_COMPANY, APP_NAME);
    m_large_icons = m_settings->value ("large-icons", SETTINGS_LARGE_ICONS).toBool();
    m_toolbar_text = m_settings->value ("toolbar-text", SETTINGS_TOOLBAR_TEXT).toBool();

    setToolbarText (m_toolbar_text);

    // Setup the read only button
    m_readonly->setEnabled (false);
    m_readonly->setVisible (false);
    m_readonly->setCheckable (true);

    // Add the actions to the toolbar
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

    // Configure the toolbar
    initialize (parent);
}

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

void ToolBar::setSaveEnabled (bool enabled) {
    m_save->setEnabled (enabled);
}

void ToolBar::setToolbarText (bool enabled) {
    QSize _icon_size;
    short _toolbar_height;

    // Set icon size
    if (m_large_icons)
        _icon_size = QSize (32, 32);

    else
        _icon_size = QSize (24, 24);

    // Show the text and set the size of the toolbar
    if (enabled) {
        if (m_large_icons)
            _toolbar_height = MAC_OS_X ? 48 : 56;

        else
            _toolbar_height = MAC_OS_X ? 36 : 48;

        setToolButtonStyle (Qt::ToolButtonTextUnderIcon);
    }

    // Hide the text and set the size of the toolbar
    else {
        if (m_large_icons)
            _toolbar_height = MAC_OS_X ? 32 : 36;

        else
            _toolbar_height = MAC_OS_X ? 27 : 32;

        setToolButtonStyle (Qt::ToolButtonIconOnly);
    }

    // Resize the toolbar
    setIconSize (_icon_size);
    setMinimumHeight (_toolbar_height);
    setMaximumHeight (_toolbar_height);

    // Force a redrawing of the damm toolbar (works on everything)
    hide();
    show();
}

void ToolBar::initialize (Window *window) {
    Q_ASSERT (window != NULL);

    window->addToolBar (this);
    window->setUnifiedTitleAndToolBarOnMac (true);

    // Connect slots between toolbar and window
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

    // Change the icon theme when the settings are synced
    connect (window, SIGNAL (updateSettings()), this, SLOT (updateSettings()));
}

void ToolBar::updateSettings (void) {
    bool _new_value = m_settings->value ("toolbar-text", SETTINGS_TOOLBAR_TEXT).toBool();
    bool _new_sizes = m_settings->value ("large-icons", SETTINGS_LARGE_ICONS).toBool();

    // Resize and redraw the toolbar if neccessary
    if (_new_value != m_toolbar_text || m_large_icons != _new_sizes) {
        m_large_icons = _new_sizes;
        m_toolbar_text = _new_value;

        setToolbarText (_new_value);
    }

    // Hide/show the toolbar
    setVisible (m_settings->value ("toolbar-enabled", SETTINGS_TOOLBAR_ENABLED).toBool());
    setEnabled (m_settings->value ("toolbar-enabled", SETTINGS_TOOLBAR_ENABLED).toBool());

    // Set icon theme
    update_theme (m_settings->value ("icon-theme", SETTINGS_ICON_THEME).toString());
}

void ToolBar::update_theme (const QString &theme) {
    Q_ASSERT (!theme.isEmpty());

    // Decide if we use small icons or large icons
    QString size = m_large_icons ? LARGE_ICONS_SIZE : SMALL_ICONS_SIZE;

    // Load the icon theme in the resources folder
    QString path = QString ("%1/%2/%3/").arg (ICON_THEMES_PATH, theme, size);

    // Apply each icon to their respective action
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
