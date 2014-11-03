//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#include "menubar.h"

MenuBar::MenuBar (QWidget *parent) : QMenuBar (parent)
{
    setAttribute (Qt::WA_DeleteOnClose);

    createActions();
    configureActions();
    createMenubar();
    updateSettings();
}

void MenuBar::setSaveEnabled (bool enabled)
{
    f_save->setEnabled (enabled);
}

void MenuBar::connectSlots (Window *window)
{
    window->setMenuBar (this);

    // Connect slots for the file menu
    connect (f_new, SIGNAL (triggered()), window, SLOT (newFile()));
    connect (f_open, SIGNAL (triggered()), window, SLOT (open()));
    connect (f_save, SIGNAL (triggered()), window->editor(), SLOT (save()));
    connect (f_save_as, SIGNAL (triggered()), window->editor(), SLOT (saveAs()));
    connect (export_html, SIGNAL (triggered()), window->editor(),
             SLOT (exportHtml()));
    connect (export_pdf, SIGNAL (triggered()), window->editor(), SLOT (exportPdf()));
    connect (f_print, SIGNAL (triggered()), window->editor(), SLOT (print()));
    connect (f_close, SIGNAL (triggered()), window, SLOT (close()));
    connect (f_quit, SIGNAL (triggered()), qApp, SLOT (closeAllWindows()));

    // Connect slots for the edit menu
    connect (e_undo, SIGNAL (triggered()), window->editor(), SLOT (undo()));
    connect (e_redo, SIGNAL (triggered()), window->editor(), SLOT (redo()));
    connect (e_cut, SIGNAL (triggered()), window->editor(), SLOT (cut()));
    connect (e_copy, SIGNAL (triggered()), window->editor(), SLOT (copy()));
    connect (e_paste, SIGNAL (triggered()), window->editor(), SLOT (paste()));
    connect (e_select_all, SIGNAL (triggered()), window->editor(),
             SLOT (selectAll()));
    connect (e_find, SIGNAL (triggered (bool)), window, SLOT (showFindDialog()));
    connect (e_find_replace, SIGNAL (triggered()), window, SLOT (showFindReplaceDialog()));
    connect (e_read_only, SIGNAL (triggered (bool)), window,
             SLOT (setReadOnly (bool)));
    connect (window, SIGNAL (readOnlyChanged (bool)), e_read_only,
             SLOT (setChecked (bool)));

    // Connect slots for the format menu
    connect (format_font, SIGNAL (triggered()), window->editor(),
             SLOT (selectFonts()));
    connect (format_word_wrap, SIGNAL (triggered (bool)), window,
             SLOT (setWordWrap (bool)));

    // Connect slots from the view menu
    connect (v_toolbar, SIGNAL (triggered (bool)), window,
             SLOT (setToolbarEnabled (bool)));
    connect (v_statusbar, SIGNAL (triggered (bool)), window,
             SLOT (setStatusBarEnabled (bool)));
    connect (v_highlight_current_line, SIGNAL (triggered (bool)), window,
             SLOT (setHCLineEnabled (bool)));
    connect (v_line_numbers, SIGNAL (triggered (bool)), window,
             SLOT (setLineNumbersEnabled (bool)));
    connect (v_toolbar_text, SIGNAL (triggered (bool)), window,
             SLOT (setToolbarText (bool)));
    connect (this, SIGNAL (change_color (QString)), window,
             SLOT (setColorscheme (QString)));
    connect (this, SIGNAL (change_icons (QString)), window,
             SLOT (setIconTheme (QString)));
    connect (this, SIGNAL (change_syntax (QString)), window->editor(),
             SLOT (setSyntaxLanguage (QString)));

    // Connect the slots for the help menu
    connect (h_about_qt, SIGNAL (triggered()), qApp, SLOT (aboutQt()));
    connect (h_about_thunderpad, SIGNAL (triggered()), window,
             SLOT (aboutThunderpad()));
    connect (h_donate, SIGNAL (triggered()), window, SLOT (donate()));
    connect (h_feedback, SIGNAL (triggered()), window, SLOT (sendFeedback()));
    connect (h_license, SIGNAL (triggered()), window, SLOT (license()));
    connect (h_report_bug, SIGNAL (triggered()), window, SLOT (reportBug()));
    connect (h_v_help, SIGNAL (triggered()), window, SLOT (viewHelp()));
    connect (h_make_donation, SIGNAL (triggered()), window,
             SLOT (makeContribution()));
    connect (h_check_for_updates, SIGNAL (triggered()), window,
             SLOT (checkForUpdates()));
    connect (h_official_website, SIGNAL (triggered()), window,
             SLOT (officialWebsite()));
}

void MenuBar::updateSettings()
{
    QSettings settings ("Alex Spataru", "Thunderpad");
    v_toolbar->setChecked (settings.value ("toolbar-enabled", true).toBool());
    v_toolbar_text->setChecked (settings.value ("toolbar-text", MAC_OS_X).toBool());
    v_statusbar->setChecked (settings.value ("statusbar-enabled", true).toBool());
    format_word_wrap->setChecked (
        settings.value ("wordwrap-enabled", true).toBool());
    v_line_numbers->setChecked (
        settings.value ("line-numbers-enabled", true).toBool());
    v_highlight_current_line->setChecked (
        settings.value ("hc-line-enabled", true).toBool());
}

void MenuBar::createActions()
{

    // Create the file menu actions
    f_new = new QAction (tr ("New"), this);
    f_open = new QAction (tr ("Open") + "...", this);
    f_save = new QAction (tr ("Save"), this);
    f_save_as = new QAction (tr ("Save as") + "...", this);
    export_html = new QAction (tr ("HTML"), this);
    export_pdf = new QAction (tr ("PDF"), this);
    f_print = new QAction (tr ("Print") + "...", this);
    f_close = new QAction (tr ("Close current document"), this);
    f_quit = new QAction (tr ("Quit"), this);

    // Create the edit menu actions
    e_undo = new QAction (tr ("Undo"), this);
    e_redo = new QAction (tr ("Redo"), this);
    e_cut = new QAction (tr ("Cut"), this);
    e_copy = new QAction (tr ("Copy"), this);
    e_paste = new QAction (tr ("Paste"), this);
    e_select_all = new QAction (tr ("Select all"), this);
    e_find = new QAction (tr ("Find"), this);
    e_find_replace = new QAction (tr ("Find and replace"), this);
    e_read_only = new QAction (tr ("Read only"), this);

    // Create the format menu actions
    format_font = new QAction (tr ("Fonts") + "...", this);
    format_word_wrap = new QAction (tr ("Word wrap"), this);

    // Create the view menu actions
    v_toolbar = new QAction (tr ("Toolbar"), this);
    v_statusbar = new QAction (tr ("Statusbar"), this);
    v_highlight_current_line = new QAction (tr ("Highlight current line"), this);
    v_line_numbers = new QAction (tr ("Line numbers"), this);
    v_toolbar_text = new QAction (tr ("Toolbar text"), this);

    // Create the tools menu actions
    t_sort_selection = new QAction (tr ("Sort selection"), this);
    t_goto_line = new QAction (tr ("Go to line") + "...", this);
    t_hexdump = new QAction (tr ("Hexadecimal dump") + "...", this);
    t_insert_date_time = new QAction (tr ("Insert date/time"), this);

    // Create the help menu actions
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

void MenuBar::configureActions()
{
    // Set the menu roles
    f_quit->setMenuRole (QAction::QuitRole);
    h_about_qt->setMenuRole (QAction::AboutQtRole);
    h_about_thunderpad->setMenuRole (QAction::AboutRole);

    // Set the menu shortcuts
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
    e_find->setShortcut (QKeySequence::Find);
    e_find_replace->setShortcut (QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_F));
    e_read_only->setShortcut (QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_R));
    format_font->setShortcut (QKeySequence (Qt::CTRL + Qt::Key_T));
    format_word_wrap->setShortcut (QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_W));
    v_toolbar->setShortcut (QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_T));
    v_statusbar->setShortcut (QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_S));
    h_v_help->setShortcut (QKeySequence::HelpContents);

    // Configure checkable actions
    e_find->setCheckable (true);
    v_toolbar->setCheckable (true);
    v_statusbar->setCheckable (true);
    e_read_only->setCheckable (true);
    format_word_wrap->setCheckable (true);
    e_find_replace->setCheckable (true);
    v_line_numbers->setCheckable (true);
    v_toolbar_text->setCheckable (true);
    v_highlight_current_line->setCheckable (true);
}

void MenuBar::createMenubar()
{

    // Create the main menus we are adding the "&" before
    // each menu to inhibit the OS to add aditional items
    // to the menubar (such as in the Edit menu in Mac).
    m_file = addMenu (tr ("&File"));
    m_edit = addMenu (tr ("&Edit"));
    m_format = addMenu (tr ("&Format"));
    m_view = addMenu (tr ("&View"));
    m_tools = addMenu (tr ("&Tools"));
    m_help = addMenu (tr ("&Help"));

    // Create the file menu
    m_file->addAction (f_new);
    m_file->addAction (f_open);
    m_file->addSeparator();
    m_file->addAction (f_save);
    m_file->addAction (f_save_as);
    m_file->addSeparator();

    // Create the export menu
    f_export = m_file->addMenu (tr ("Export") + "...");
    f_export->addAction (export_html);
    f_export->addAction (export_pdf);

    // Continue with the file menu...
    m_file->addSeparator();
    m_file->addAction (f_print);
    m_file->addSeparator();
    m_file->addAction (f_close);
    m_file->addAction (f_quit);

    // Create the edit menu
    m_edit->addAction (e_undo);
    m_edit->addAction (e_redo);
    m_edit->addSeparator();
    m_edit->addAction (e_cut);
    m_edit->addAction (e_copy);
    m_edit->addAction (e_paste);
    m_edit->addSeparator();
    m_edit->addAction (e_select_all);
    m_edit->addSeparator();
    m_edit->addAction (e_find);
    m_edit->addAction (e_find_replace);
    m_edit->addSeparator();
    m_edit->addAction (e_read_only);

    // Create the format menu
    m_format->addAction (format_font);
    m_format->addAction (format_word_wrap);

    // Create the visible menu
    m_view->addAction (v_toolbar);
    m_view->addAction (v_statusbar);
    m_view->addSeparator();

    // Create the advanced menu
    v_advanced = m_view->addMenu (tr ("Advanced"));
    v_advanced->addAction (v_highlight_current_line);
    v_advanced->addAction (v_line_numbers);
    v_advanced->addSeparator();
    v_advanced->addAction (v_toolbar_text);
    v_advanced->addSeparator();

    // Create the color schemes menu
    color_schemes = v_advanced->addMenu (tr ("Color schemes"));
    QActionGroup *color_schemes_group = new QActionGroup (this);
    QSignalMapper *color_schemes_mapper = new QSignalMapper (this);
    QSettings settings ("Alex Spataru", "Thunderpad");

    // Read all the registered color schemes in the resources
    QDir color_schemes_dir (":/color-schemes/");
    QStringList color_schemes_list =
        color_schemes_dir.entryList (QStringList ("*.xml"));
    connect (color_schemes_mapper, SIGNAL (mapped (QString)), this,
             SIGNAL (change_color (QString)));

    // Create a new action for each registered color scheme
    for (int i = 0; color_schemes_list.count() > i; ++i)
    {

        // Get the name of the color scheme
        QString name = color_schemes_list.at (i);
        name.replace (".xml", "");

        // Create a new action
        QAction *action = new QAction (name, this);

        // Configure the action
        action->setCheckable (true);
        color_schemes->addAction (action);
        color_schemes_group->addAction (action);
        color_schemes_mapper->setMapping (action, action->text());
        connect (action, SIGNAL (triggered()), color_schemes_mapper, SLOT (map()));

        // Check the icon if necessary
        if (settings.value ("color-scheme", "Light").toString() == name)
            action->setChecked (true);
    }

    // Create the syntax highlighter menu in a process similar to the color
    // schemes menu
    syntax_languages = v_advanced->addMenu (tr ("Syntax highlighting"));
    QActionGroup *syntax_languages_group = new QActionGroup (this);
    QSignalMapper *syntax_languages_mapper = new QSignalMapper (this);
    connect (syntax_languages_mapper, SIGNAL (mapped (QString)), this,
             SIGNAL (change_syntax (QString)));

    // Read all the registered color schemes in the resources
    QDir syntax_languages_dir (":/languages/");
    QStringList syntax_languages_list =
        syntax_languages_dir.entryList (QStringList ("*.xml"));

    // Create a new action for each registered language
    for (int i = 0; syntax_languages_list.count() > i; ++i)
    {

        // Get the name of the color scheme
        QString name = syntax_languages_list.at (i);
        name.replace (".xml", "");

        // Create a new action
        QAction *action = new QAction (name, this);

        // Configure the action
        action->setCheckable (true);
        syntax_languages->addAction (action);
        syntax_languages_group->addAction (action);
        syntax_languages_mapper->setMapping (action, action->text());
        connect (action, SIGNAL (triggered()), syntax_languages_mapper, SLOT (map()));
    }

    // Create the icon theme menu using a process similar to the color schemes
    // menu
    v_icon_theme = m_view->addMenu (tr ("Icon themes"));
    QActionGroup *icon_themes_group = new QActionGroup (this);
    QSignalMapper *icon_themes_mapper = new QSignalMapper (this);
    connect (icon_themes_mapper, SIGNAL (mapped (QString)), this,
             SIGNAL (change_icons (QString)));

    // Read all the registered color schemes in the resources
    QDir icon_themes_dir (":/icons/themes/");
    QStringList icon_themes_list = icon_themes_dir.entryList();

    // Create a new action for each registered icon theme
    for (int i = 0; icon_themes_list.count() > i; ++i)
    {
        // Get the name of the current theme and create the action
        QAction *action = new QAction (icon_themes_list.at (i), this);

        // Configure the action
        action->setCheckable (true);
        v_icon_theme->addAction (action);
        icon_themes_group->addAction (action);
        icon_themes_mapper->setMapping (action, action->text());
        connect (action, SIGNAL (triggered()), icon_themes_mapper, SLOT (map()));

        // Check the icon if necessary
        if (settings.value ("icon-theme", "Silk").toString() == action->text())
            action->setChecked (true);
    }

    // Create the tools menu
    m_tools->addAction (t_sort_selection);
    m_tools->addAction (t_goto_line);
    m_tools->addSeparator();
    m_tools->addAction (t_hexdump);
    m_tools->addSeparator();
    m_tools->addAction (t_insert_date_time);

    // Create the help menu
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
