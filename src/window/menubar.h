//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex_spataru@outlook.com>
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

#include <QDir>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QSettings>
#include <QStringList>
#include <QKeySequence>
#include <QSignalMapper>

#include "window.h"

/*!
 * \class MenuBar
 *
 * The \c MenuBar class is in charge of creating and configuring
 * a new QMenuBar widget to be used with the \c Window class.
 *
 * This class creates the neccessary actions and connects the signals/slots
 * automatically with the initialize() function.
 */

class MenuBar : public QMenuBar {
    Q_OBJECT

public:
    MenuBar(Window *parent = 0);

    /*!
   * \brief setSaveEnabled
   * \param enabled
   *
   * Disables (or enables) the save action based on
   * the current status of the document
   */

    void setSaveEnabled(bool enabled);

signals:

    /*!
   * \brief change_icons
   * \param theme
   *
   * This signal is emitted when the user clicks
   * any of the actions related to the "toolbar icons" section
   */

    void change_icons(const QString &theme);

    /*!
   * \brief change_color
   * \param theme
   *
   * This signal is emitted when the user clicks
   * any of the actions related to the "themes" section
   */

    void change_color(const QString &theme);

    /*!
   * \brief change_syntax
   * \param lang
   * This signal is emitted when the user clicks
   * any of the actions related to the "syntax highlighter" section
   */

    void change_syntax(const QString &lang);

private slots:
    void createActions(void);
    void createMenubar(void);
    void updateSettings(void);
    void configureActions(void);
    void setReadOnly(bool ro);
    void initialize(Window *window);

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

    QMenu *v_advanced;
    QAction *v_highlight_current_line;
    QAction *v_line_numbers;

    QMenu *v_appearance;
    QAction *v_toolbar_text;
    QAction *v_large_toolbar_icons;

    QMenu *color_schemes;
    QMenu *syntax_languages;
    QMenu *v_icon_theme;

    QAction *t_sort_selection;
    QAction *t_goto_line;
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
