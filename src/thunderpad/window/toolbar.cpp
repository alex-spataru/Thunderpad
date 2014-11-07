//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#include "toolbar.h"

ToolBar::ToolBar (QWidget *parent) : QToolBar (parent)
{
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
    m_replace = new QAction (tr ("Replace"), this);
    m_readonly = new QAction (tr ("Read only"), this);

    // Load the settings
    m_settings = new QSettings ("Alex Spataru", "Thunderpad");
    updateSettings();

    // Show the toolbar
    m_old_value = m_settings->value ("toolbar-text", false).toBool();
    setToolbarText (m_old_value);

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
    addAction (m_replace);
    addAction (m_readonly);
}

void ToolBar::setReadOnly (bool ro)
{
    m_readonly->setChecked (ro);
    m_readonly->setEnabled (ro);
    m_readonly->setVisible (ro);
}

void ToolBar::setSaveEnabled (bool enabled)
{
    m_save->setEnabled (enabled);
}

void ToolBar::setToolbarText (bool enabled)
{
    short _toolbar_size;

    // Show the text and set the size of the toolbar
    if (enabled)
    {
        _toolbar_size = MAC_OS_X ? 36 : 48;
        setToolButtonStyle (Qt::ToolButtonTextUnderIcon);
    }

    // Hide the text and set the size of the toolbar
    else
    {
        _toolbar_size = MAC_OS_X ? 24 : 32;
        setToolButtonStyle (Qt::ToolButtonIconOnly);
    }

    // Resize the toolbar
    setMinimumHeight (_toolbar_size);
    setMaximumHeight (_toolbar_size);

    // Re-draw the toolbar (using the update() function) hides
    // the menubar (at least in OS X)
    hide();
    show();
}

void ToolBar::connectSlots (Window *window)
{
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

    // Change the icon theme when the settings are synced
    connect (window, SIGNAL (updateSettings()), this, SLOT (updateSettings()));
}

void ToolBar::updateSettings()
{
    // Set the icon theme
    update_theme (m_settings->value ("icon-theme", "Silk").toString());

    // Hide/show the toolbar
    setVisible (m_settings->value ("toolbar-enabled", true).toBool());
    setEnabled (m_settings->value ("toolbar-enabled", true).toBool());

    // Update the toolbar style, we use the "if" condition to avoid
    // resizing and redrawing the toolbar each time that the user changes
    bool _new_value = m_settings->value ("toolbar-text", MAC_OS_X).toBool();

    if (_new_value != m_old_value)
    {
        setToolbarText (_new_value);
        m_old_value = _new_value;
    }
}

void ToolBar::update_theme (const QString &theme)
{
    Q_ASSERT (!theme.isEmpty());

    // Load the icon theme in the resources folder
    QString path = ":/icons/themes/" + theme + "/";

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
    m_replace->setIcon (QIcon (path + "replace.png"));
    m_readonly->setIcon (QIcon (path + "lock.png"));
}
