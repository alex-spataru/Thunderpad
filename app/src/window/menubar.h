//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#ifndef MENUBAR_H
#define MENUBAR_H

#include <qdir.h>
#include <qmenu.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qsettings.h>
#include <qstringlist.h>
#include <qkeysequence.h>
#include <qsignalmapper.h>

#include "window.h"

/*!
 * \class MenuBar
 *
 * The \c MenuBar class is in charge of creating and configuring
 * a new QMenuBar widget to be used with the \c Window class.
 *
 * This class creates the neccessary actions and connects the signals/slots
 * automatically with the connectSlots() function.
 */

class MenuBar : public QMenuBar
{
        Q_OBJECT

    public:
        MenuBar (QWidget *parent = 0);
        void setSaveEnabled (bool enabled);
        void connectSlots (Window *window);

        public
    slots:
        void updateSettings();

        private
    slots:
        void createActions();
        void createMenubar();
        void configureActions();

    signals:
        void change_icons (const QString &theme);
        void change_color (const QString &theme);
        void change_syntax (const QString &lang);

    private:
        QMenu *m_file;
        QMenu *m_edit;
        QMenu *m_format;
        QMenu *m_view;
        QMenu *m_tools;
        QMenu *m_help;

        QAction *f_new;
        QAction *f_open;
        QAction *f_save;
        QAction *f_save_as;

        QMenu *f_export;
        QAction *export_html;
        QAction *export_pdf;

        QAction *f_print;
        QAction *f_close;
        QAction *f_quit;

        QAction *e_undo;
        QAction *e_redo;
        QAction *e_cut;
        QAction *e_copy;
        QAction *e_paste;
        QAction *e_select_all;
        QAction *e_find;
        QAction *e_find_replace;
        QAction *e_read_only;

        QAction *format_font;
        QAction *format_word_wrap;

        QAction *v_toolbar;
        QAction *v_statusbar;

        QMenu *v_advanced;
        QAction *v_highlight_current_line;
        QAction *v_line_numbers;
        QAction *v_toolbar_text;

        QMenu *color_schemes;
        QMenu *syntax_languages;
        QMenu *v_icon_theme;

        QAction *t_sort_selection;
        QAction *t_goto_line;
        QAction *t_hexdump;
        QAction *t_insert_date_time;

        QAction *h_about_thunderpad;
        QAction *h_about_qt;
        QAction *h_donate;
        QAction *h_feedback;
        QAction *h_license;
        QAction *h_v_help;
        QAction *h_report_bug;
        QAction *h_make_donation;
        QAction *h_check_for_updates;
        QAction *h_official_website;
};

#endif
