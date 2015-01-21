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

#ifndef SearchDialog_H
#define SearchDialog_H

#include <QLabel>
#include <QDialog>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QGridLayout>

#include "editor.h"
#include "window.h"

class SearchDialog : public QDialog
{
        Q_OBJECT

    public:
        SearchDialog (Window *parent = 0);

    private slots:
        void search (void);
        void replaceAll (void);
        void replaceFirstOccurrence (void);

    private:
        Editor *m_text_edit;

        QLabel *ui_find_label;
        QGridLayout *ui_layout;
        QLabel *ui_replace_label;
        QLineEdit *ui_find_lineedit;
        QPushButton *ui_done_button;
        QPushButton *ui_replace_button;
        QLineEdit *ui_replace_lineedit;
        QSpacerItem *ui_vertical_spacer;
        QCheckBox *ui_match_case_checkbox;
        QCheckBox *ui_whole_words_checkbox;
        QPushButton *ui_replace_all_button;
};

#endif
