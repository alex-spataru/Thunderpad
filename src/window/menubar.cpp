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

#include <QDir>
#include <QMenu>
#include <QAction>
#include <QSettings>
#include <QKeySequence>
#include <QApplication>
#include <QSignalMapper>

#include "editor.h"
#include "window.h"
#include "menubar.h"
#include "platform.h"
#include "defaults.h"

/*!
 * \class MenuBar
 *
 * The \c MenuBar class is in charge of creating and configuring
 * a new QMenuBar widget to be used with the \c Window class.
 *
 * This class creates the neccessary actions and connects the signals/slots
 * automatically with the initialize() function.
 */

/*!
 * \internal
 * Initializes and configures the widget
 */

MenuBar::MenuBar (Window *parent) : QMenuBar (parent) {
    createActions();
    configureActions();
    createMenubar();

    initialize (parent);
}

/*!
 * Enables or disables the save item in the "File" menu
 * based on the value of \a {enabled}.
 */

void MenuBar::setSaveEnabled (bool enabled) {
    f_save->setEnabled (enabled);
}

/*!
 * Connects the menubar actions to their respective
 * slots in the main window and the text editor
 */

void MenuBar::initialize (Window *window) {
    Q_ASSERT (window != NULL);

    window->setMenuBar (this);

    //
    // Connect slots for the file menu
    //
    connect (f_new, SIGNAL (triggered()), window, SLOT (newFile()));
    connect (f_open, SIGNAL (triggered()), window, SLOT (open()));
    connect (f_save, SIGNAL (triggered()), window->editor(), SLOT (save()));
    connect (f_save_as, SIGNAL (triggered()), window->editor(), SLOT (saveAs()));
    connect (export_pdf, SIGNAL (triggered()), window->editor(), SLOT (exportPdf()));
    connect (f_print, SIGNAL (triggered()), window->editor(), SLOT (print()));
    connect (f_close, SIGNAL (triggered()), window, SLOT (close()));
    connect (f_quit, SIGNAL (triggered()), qApp, SLOT (closeAllWindows()));

    //
    // Connect slots for the edit menu
    //
    connect (e_undo, SIGNAL (triggered()), window->editor(), SLOT (undo()));
    connect (e_redo, SIGNAL (triggered()), window->editor(), SLOT (redo()));
    connect (e_cut, SIGNAL (triggered()), window->editor(), SLOT (cut()));
    connect (e_copy, SIGNAL (triggered()), window->editor(), SLOT (copy()));
    connect (e_paste, SIGNAL (triggered()), window->editor(), SLOT (paste()));
    connect (e_select_all, SIGNAL (triggered()), window->editor(), SLOT (selectAll()));
    connect (e_find_replace, SIGNAL (triggered()), window, SLOT (showFindReplaceDialog()));
    connect (e_read_only, SIGNAL (triggered (bool)), window, SLOT (setReadOnly (bool)));
    connect (window, SIGNAL (readOnlyChanged (bool)), this, SLOT (setReadOnly (bool)));

    //
    // Connect slots for the tools menu
    //
    connect (t_goto_line, SIGNAL (triggered()), window->editor(), SLOT (goToLine()));
    connect (t_sort_selection, SIGNAL (triggered()), window->editor(), SLOT (sortSelection()));
    connect (t_insert_date_time, SIGNAL (triggered()), window->editor(), SLOT (insertDateTime()));
    connect (t_document_information, SIGNAL (triggered()), window->editor(), SLOT (documentInfo()));

    //
    // Connect slots for the format menu
    //
    connect (format_font, SIGNAL (triggered()), window->editor(), SLOT (selectFonts()));
    connect (format_word_wrap, SIGNAL (triggered (bool)), window, SLOT (setWordWrap (bool)));

    //
    // Connect slots from the view menu
    //
    connect (v_toolbar, SIGNAL (triggered (bool)), window, SLOT (setToolbarEnabled (bool)));
    connect (v_statusbar, SIGNAL (triggered (bool)), window, SLOT (setStatusBarEnabled (bool)));
    connect (v_zoom_in, SIGNAL (triggered()), window->editor(), SLOT (zoomIn()));
    connect (v_zoom_out, SIGNAL (triggered()), window->editor(), SLOT (zoomOut()));
    connect (v_zoom_reset, SIGNAL (triggered()), window->editor(), SLOT (resetZoom()));
    connect (v_highlight_current_line, SIGNAL (triggered (bool)), window, SLOT (setHCLineEnabled (bool)));
    connect (v_line_numbers, SIGNAL (triggered (bool)), window, SLOT (setLineNumbersEnabled (bool)));
    connect (v_toolbar_text, SIGNAL (triggered (bool)), window, SLOT (setToolbarText (bool)));
    connect (v_large_toolbar_icons, SIGNAL (triggered (bool)), window, SLOT (setUseLargeIcons (bool)));
    connect (this, SIGNAL (colorChanged (QString)), window, SLOT (setColorscheme (QString)));
    connect (this, SIGNAL (iconsChanged (QString)), window, SLOT (setIconTheme (QString)));

    //
    // Connect the slots for the help menu
    //
    connect (h_about_qt, SIGNAL (triggered()), qApp, SLOT (aboutQt()));
    connect (h_about_thunderpad, SIGNAL (triggered()), window, SLOT (aboutThunderpad()));
    connect (h_donate, SIGNAL (triggered()), window, SLOT (donate()));
    connect (h_feedback, SIGNAL (triggered()), window, SLOT (sendFeedback()));
    connect (h_license, SIGNAL (triggered()), window, SLOT (license()));
    connect (h_report_bug, SIGNAL (triggered()), window, SLOT (reportBug()));
    connect (h_v_help, SIGNAL (triggered()), window, SLOT (viewHelp()));
    connect (h_make_donation, SIGNAL (triggered()), window, SLOT (makeContribution()));
    connect (h_check_for_updates, SIGNAL (triggered()), window, SIGNAL (checkForUpdates()));
    connect (h_official_website, SIGNAL (triggered()), window, SLOT (officialWebsite()));

    //
    // Sync settings
    //
    connect (window, SIGNAL (updateSettings()), this, SLOT (updateSettings()));
}

/*!
 * Changes the checked state of the actions that change in any way the settings
 * of the application.
 * This function is called automatically when a setting is changed in order
 * to maintain the menubar actions in sync with the current settings of
 * the application.
 */

void MenuBar::updateSettings (void) {
    v_toolbar->setChecked (settings()->value ("toolbar-enabled", SETTINGS_TOOLBAR_ENABLED).toBool());
    v_toolbar_text->setChecked (settings()->value ("toolbar-text", SETTINGS_TOOLBAR_TEXT).toBool());
    v_statusbar->setChecked (settings()->value ("statusbar-enabled", SETTINGS_STATUSBAR_ENABLED).toBool());
    format_word_wrap->setChecked (settings()->value ("wordwrap-enabled", SETTINGS_WORD_WRAP_ENABLED).toBool());
    v_large_toolbar_icons->setChecked (settings()->value ("large-icons", SETTINGS_LARGE_ICONS).toBool());
    v_line_numbers->setChecked (settings()->value ("line-numbers-enabled", SETTINGS_LINE_NUMBERS).toBool());
    v_highlight_current_line->setChecked (settings()->value ("hc-line-enabled", SETTINGS_CARET_LINE).toBool());
}

/*!
 * Initializes the menubar actions and sets their respective strings
 */

void MenuBar::createActions (void) {
    //
    // Create the file menu actions
    //
    f_new = new QAction (tr ("New"), this);
    f_open = new QAction (tr ("Open") + "...", this);
    f_save = new QAction (tr ("Save"), this);
    f_save_as = new QAction (tr ("Save as") + "...", this);
    export_html = new QAction (tr ("HTML"), this);
    export_pdf = new QAction (tr ("PDF"), this);
    f_print = new QAction (tr ("Print") + "...", this);
    f_close = new QAction (tr ("Close current document"), this);
    f_quit = new QAction (tr ("Quit"), this);

    //
    // Create the edit menu actions
    //
    e_undo = new QAction (tr ("Undo"), this);
    e_redo = new QAction (tr ("Redo"), this);
    e_cut = new QAction (tr ("Cut"), this);
    e_copy = new QAction (tr ("Copy"), this);
    e_paste = new QAction (tr ("Paste"), this);
    e_select_all = new QAction (tr ("Select all"), this);
    e_find_replace = new QAction (tr ("Find/Replace"), this);
    e_read_only = new QAction (tr ("Read only"), this);

    //
    // Create the format menu actions
    //
    format_font = new QAction (tr ("Fonts") + "...", this);
    format_word_wrap = new QAction (tr ("Word wrap"), this);

    //
    // Create the view menu actions
    //
    v_toolbar = new QAction (tr ("Toolbar"), this);
    v_statusbar = new QAction (tr ("Statusbar"), this);
    v_zoom_in = new QAction (tr ("Zoom in"), this);
    v_zoom_out = new QAction (tr ("Zoom out"), this);
    v_zoom_reset = new QAction (tr ("Reset zoom"), this);
    v_highlight_current_line = new QAction (tr ("Highlight current line"), this);
    v_line_numbers = new QAction (tr ("Show line numbers"), this);
    v_large_toolbar_icons = new QAction (tr ("Large toolbar icons"), this);
    v_toolbar_text = new QAction (tr ("Display text under toolbar icons"), this);

    //
    // Create the tools menu actions
    //
    t_sort_selection = new QAction (tr ("Sort selection"), this);
    t_goto_line = new QAction (tr ("Go to line") + "...", this);
    t_insert_date_time = new QAction (tr ("Insert date/time"), this);
    t_document_information = new QAction (tr ("Document information"), this);

    //
    // Create the help menu actions
    //
    h_about_qt = new QAction (tr ("About Qt") + "...", this);
    h_feedback = new QAction (tr ("Send feedback") + "...", this);
    h_donate = new QAction (tr ("Donate") + "...", this);
    h_about_thunderpad = new QAction (tr ("About Thunderpad"), this);
    h_license = new QAction (tr ("License") + "...", this);
    h_v_help = new QAction (tr ("View help") + "...", this);
    h_report_bug = new QAction (tr ("Report a bug") + "...", this);
    h_make_donation = new QAction (tr ("Contribute") + "...", this);
    h_check_for_updates = new QAction (tr ("Check for updates") + "...", this);
    h_official_website = new QAction (tr ("Website") + "...", this);
}

/*!
 * Creates standard shortcuts for each action and enables some selected
 * actions to display a checked or unchecked state.
 */

void MenuBar::configureActions (void) {
    //
    // Set the menu roles
    //
    f_quit->setMenuRole (QAction::QuitRole);
    h_about_qt->setMenuRole (QAction::AboutQtRole);
    h_about_thunderpad->setMenuRole (QAction::AboutRole);

    //
    // Set the menu shortcuts
    //
    f_new->setShortcut (QKeySequence::New);
    f_open->setShortcut (QKeySequence::Open);
    f_save->setShortcut (QKeySequence::Save);
    f_save_as->setShortcut (QKeySequence::SaveAs);
    f_print->setShortcut (QKeySequence::Print);
    f_close->setShortcut (QKeySequence::Close);
    f_quit->setShortcut (QKeySequence::Quit);
    e_undo->setShortcut (QKeySequence::Undo);
    e_redo->setShortcut (QKeySequence::Redo);
    e_cut->setShortcut (QKeySequence::Cut);
    e_copy->setShortcut (QKeySequence::Copy);
    e_paste->setShortcut (QKeySequence::Paste);
    e_select_all->setShortcut (QKeySequence::SelectAll);
    e_find_replace->setShortcut (QKeySequence::Find);
    e_read_only->setShortcut (QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_R));
    format_font->setShortcut (QKeySequence (Qt::CTRL + Qt::Key_T));
    format_word_wrap->setShortcut (QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_W));
    v_toolbar->setShortcut (QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_T));
    v_statusbar->setShortcut (QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_S));
    h_v_help->setShortcut (QKeySequence::HelpContents);
    v_zoom_in->setShortcut (QKeySequence::ZoomIn);
    v_zoom_out->setShortcut (QKeySequence::ZoomOut);
    v_zoom_reset->setShortcut (QKeySequence (Qt::CTRL + Qt::Key_0));

    //
    // Configure checkable actions
    //
    v_toolbar->setCheckable (true);
    v_statusbar->setCheckable (true);
    e_read_only->setCheckable (true);
    format_word_wrap->setCheckable (true);
    v_line_numbers->setCheckable (true);
    v_toolbar_text->setCheckable (true);
    v_large_toolbar_icons->setCheckable (true);
    v_highlight_current_line->setCheckable (true);
}

/*!
 * Organizes the menubar actions in different categories, such as:
 * \list
 * \o File
 * \o Edit
 * \o Format
 * \o Etc.
 * \endlist
 */

void MenuBar::createMenubar (void) {
    //
    // Create the main menus we are adding the "&" before
    // each menu to inhibit the OS to add aditional items
    // to the menubar (such as in the Edit menu in Mac).
    //
    m_file = addMenu (tr ("&File"));
    m_edit = addMenu (tr ("&Edit"));
    m_format = addMenu (tr ("&Format"));
    m_view = addMenu (tr ("&View"));
    m_tools = addMenu (tr ("&Tools"));
    m_help = addMenu (tr ("&Help"));

    //
    // Create the file menu
    //
    m_file->addAction (f_new);
    m_file->addAction (f_open);
    m_file->addSeparator();
    m_file->addAction (f_save);
    m_file->addAction (f_save_as);
    m_file->addSeparator();

    //
    // Create the export menu
    //
    f_export = m_file->addMenu (tr ("Export") + "...");
    f_export->addAction (export_html);
    f_export->addAction (export_pdf);

    //
    // Continue with the file menu...
    //
    m_file->addSeparator();
    m_file->addAction (f_print);
    m_file->addSeparator();
    m_file->addAction (f_close);
    m_file->addAction (f_quit);

    //
    // Create the edit menu
    //
    m_edit->addAction (e_undo);
    m_edit->addAction (e_redo);
    m_edit->addSeparator();
    m_edit->addAction (e_cut);
    m_edit->addAction (e_copy);
    m_edit->addAction (e_paste);
    m_edit->addSeparator();
    m_edit->addAction (e_select_all);
    m_edit->addSeparator();
    m_edit->addAction (e_find_replace);
    m_edit->addSeparator();
    m_edit->addAction (e_read_only);

    //
    // Create the format menu
    //
    m_format->addAction (format_font);
    m_format->addAction (format_word_wrap);

    //
    // Create the visible menu
    //
    m_view->addAction (v_toolbar);
    m_view->addAction (v_statusbar);
    m_view->addSeparator();

    //
    // Create the zoom menu
    //
    v_zoom = m_view->addMenu (tr ("Zoom"));
    v_zoom->addAction (v_zoom_in);
    v_zoom->addAction (v_zoom_out);
    v_zoom->addSeparator();
    v_zoom->addAction (v_zoom_reset);
    m_view->addSeparator();

    //
    // Create the advanced menu
    //
    v_appearance = m_view->addMenu (tr ("Appearance"));
    v_appearance->addAction (v_highlight_current_line);
    v_appearance->addAction (v_line_numbers);
    v_appearance->addSeparator();
    v_appearance->addAction (v_large_toolbar_icons);
    v_appearance->addAction (v_toolbar_text);

    //
    // Create the icon theme menu
    //
    v_icon_theme = m_view->addMenu (tr ("Icon themes"));
    QActionGroup *icon_themes_group = new QActionGroup (this);
    QSignalMapper *icon_themes_mapper = new QSignalMapper (this);
    connect (icon_themes_mapper, SIGNAL (mapped (QString)), this, SIGNAL (iconsChanged (QString)));

    //
    // Read all the registered icons in the resources
    //
    QDir icon_themes_dir (":/images/themes/");
    QStringList icon_themes_list = icon_themes_dir.entryList();

    //
    // Create a new action for each registered icon theme
    //
    for (int i = 0; icon_themes_list.count() > i; ++i) {
        //
        // Get the name of the current theme and create the action
        //
        QAction *_action = new QAction (icon_themes_list.at (i), this);

        //
        // Configure the action
        //
        _action->setCheckable (true);
        v_icon_theme->addAction (_action);
        icon_themes_group->addAction (_action);
        icon_themes_mapper->setMapping (_action, _action->text());
        connect (_action, SIGNAL (triggered()), icon_themes_mapper, SLOT (map()));

        //
        // Check the icon if necessary
        //
        if (settings()->value ("icon-theme", SETTINGS_ICON_THEME).toString() == _action->text())
            _action->setChecked (true);
    }

    //
    // Create the tools menu
    //
    m_tools->addAction (t_sort_selection);
    m_tools->addAction (t_goto_line);
    m_tools->addSeparator();
    m_tools->addAction (t_insert_date_time);
    m_tools->addAction (t_document_information);

    //
    // Create the help menu
    //
    m_help->addAction (h_about_qt);
    m_help->addAction (h_about_thunderpad);
    m_help->addSeparator();
    m_help->addAction (h_donate);
    m_help->addSeparator();
    m_help->addAction (h_feedback);
    m_help->addSeparator();
    m_help->addAction (h_license);
    m_help->addAction (h_v_help);
    m_help->addAction (h_report_bug);
    m_help->addSeparator();
    m_help->addAction (h_make_donation);
    m_help->addSeparator();
    m_help->addAction (h_check_for_updates);
    m_help->addAction (h_official_website);
}

/*!
 * Enables or disables menubar actions that can directly modify the
 * contents of the text editor based on the value of \a {ro}
 */

void MenuBar::setReadOnly (bool ro) {
    //
    // Disable/Enable undo/redo actions
    //
    e_undo->setEnabled (!ro);
    e_redo->setEnabled (!ro);

    //
    // Disable/Enable the cut, copy and
    // paste actions.
    //
    e_cut->setEnabled (!ro);
    e_copy->setEnabled (!ro);
    e_paste->setEnabled (!ro);

    //
    // Change the state of the read only
    // action.
    //
    e_read_only->setChecked (ro);
}

/*!
 * Allows the class to access the application settings
 */

QSettings *MenuBar::settings (void) const {
    return new QSettings (APP_COMPANY, APP_NAME);
}

