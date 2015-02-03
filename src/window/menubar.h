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

#ifndef MENUBAR_H
#define MENUBAR_H

class QMenu;
class Window;
class QAction;
class QSettings;

#include <QMenuBar>

class MenuBar : public QMenuBar {
        Q_OBJECT

    public:
        MenuBar (Window *parent = 0);
        void setSaveEnabled (bool enabled);

    signals:
        void iconsChanged (const QString &theme);
        void colorChanged (const QString &theme);
        void syntaxChanged (const QString &lang);

    private slots:
        void createActions (void);
        void createMenubar (void);
        void updateSettings (void);
        void configureActions (void);
        void setReadOnly (bool ro);
        void initialize (Window *window);

    private:
        QSettings *m_settings;

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
        QAction *e_find_replace;
        QAction *e_read_only;

        QAction *format_font;
        QAction *format_word_wrap;

        QAction *v_toolbar;
        QAction *v_statusbar;

        QMenu *v_zoom;
        QAction *v_zoom_in;
        QAction *v_zoom_out;
        QAction *v_zoom_reset;

        QMenu *v_appearance;
        QAction *v_highlight_current_line;
        QAction *v_line_numbers;
        QAction *v_toolbar_text;
        QAction *v_large_toolbar_icons;

        QMenu *v_icon_theme;

        QAction *t_sort_selection;
        QAction *t_goto_line;
        QAction *t_insert_date_time;
        QAction *t_document_information;

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
