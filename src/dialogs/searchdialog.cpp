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

#include "searchdialog.h"

SearchDialog::SearchDialog (Window *parent) : QDialog (parent)
{
    QIcon _blank;
    setWindowIcon (_blank);

    // Configure the window
    setWindowTitle (tr ("Find/Replace"));
    setWindowModality (Qt::WindowModal);
    setWindowFlags (Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    // Get access to the text editor
    m_text_edit = parent->editor();

    // Initialize UI items
    ui_find_label = new QLabel (this);
    ui_layout = new QGridLayout (this);
    ui_replace_label = new QLabel (this);
    ui_find_lineedit = new QLineEdit (this);
    ui_done_button = new QPushButton (this);
    ui_replace_button = new QPushButton (this);
    ui_replace_lineedit = new QLineEdit (this);
    ui_match_case_checkbox = new QCheckBox (this);
    ui_whole_words_checkbox = new QCheckBox (this);
    ui_replace_all_button = new QPushButton (this);

    // Set the text of each widget
    ui_done_button->setText (tr ("Done"));
    ui_replace_button->setText (tr ("Replace"));
    ui_find_label->setText (tr ("Find what") + ":");
    ui_replace_all_button->setText (tr ("Replace all"));
    ui_match_case_checkbox->setText (tr ("Match case"));
    ui_replace_label->setText (tr ("Replace with") + ":");
    ui_whole_words_checkbox->setText (tr ("Whole words only"));

    ui_vertical_spacer = new QSpacerItem (20, 40,
                                          QSizePolicy::Minimum,
                                          QSizePolicy::Expanding);

    // Arrange the widgets in a nice layout...
    ui_layout->setSpacing (10);
    ui_layout->addWidget (ui_find_label,           1, 1, 1, 1);
    ui_layout->addWidget (ui_done_button,          6, 4, 3, 4);
    ui_layout->addWidget (ui_find_lineedit,        1, 2, 1, 2);
    ui_layout->addWidget (ui_replace_label,        2, 1, 2, 1);
    ui_layout->addWidget (ui_replace_button,       1, 4, 1, 4);
    ui_layout->addWidget (ui_replace_lineedit,     2, 2, 2, 2);
    ui_layout->addWidget (ui_replace_all_button,   2, 4, 2, 4);
    ui_layout->addWidget (ui_match_case_checkbox,  4, 1, 3, 1);
    ui_layout->addWidget (ui_whole_words_checkbox, 6, 1, 3, 1);
    ui_layout->addItem (ui_vertical_spacer,        3, 1, 3, 1);

    // Inhibit resizing
    layout()->setSizeConstraint (QLayout::SetFixedSize);

    // Disable replace functions
    ui_replace_button->setEnabled (false);
    ui_replace_lineedit->setEnabled (false);
    ui_replace_all_button->setEnabled (false);

    // Connect the UI with the dialog logic
    connect (ui_done_button, SIGNAL (clicked()), this, SLOT (hide()));
    connect (ui_match_case_checkbox, SIGNAL (clicked()), this, SLOT (search()));
    connect (ui_whole_words_checkbox, SIGNAL (clicked()), this, SLOT (search()));
    connect (ui_replace_all_button, SIGNAL (clicked()), this, SLOT (replaceAll()));
    connect (ui_find_lineedit, SIGNAL (textChanged (QString)), this, SLOT (search()));
    connect (ui_replace_button, SIGNAL (clicked()), this, SLOT (replaceFirstOccurrence()));
}

void SearchDialog::search (void)
{
}

void SearchDialog::replaceAll (void)
{
}

void SearchDialog::replaceFirstOccurrence (void)
{
}
