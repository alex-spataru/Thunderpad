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

/*!
 * \class SearchDialog
 * \brief Implements a simple search dialog
 *
 * The \c SearchDialog class implements a simple search
 * dialog that allows the user to:
 *
 * \list
 * \o Search for a string in the \c Editor using different options
 * \o Navigate through search results easily
 * \o Replace the selected search match
 * \o Replace all matches of a search querry
 * \endlist
 */

SearchDialog::SearchDialog (Window *parent) : QDialog (parent) {
    QIcon _blank;
    setWindowIcon (_blank);

    //
    // Configure the window
    //
    setWindowTitle (tr ("Find/Replace"));
    setWindowModality (Qt::WindowModal);
    setWindowFlags (Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    //
    // Get access to the text editor
    //
    m_text_edit = parent->editor();

    //
    // Initialize UI items
    //
    ui_find_label = new QLabel (this);
    ui_layout = new QGridLayout (this);
    ui_replace_label = new QLabel (this);
    ui_find_lineedit = new QLineEdit (this);
    ui_done_button = new QPushButton (this);
    ui_next_button = new QPushButton (this);
    ui_replace_button = new QPushButton (this);
    ui_replace_lineedit = new QLineEdit (this);
    ui_match_case_checkbox = new QCheckBox (this);
    ui_whole_words_checkbox = new QCheckBox (this);
    ui_replace_all_button = new QPushButton (this);
    ui_regex_search_checkbox = new QCheckBox (this);

    //
    // Set the text of each widget
    //
    ui_done_button->setText (tr ("Done"));
    ui_next_button->setText (tr ("Find next"));
    ui_replace_button->setText (tr ("Replace"));
    ui_find_label->setText (tr ("Find what") + ":");
    ui_replace_all_button->setText (tr ("Replace all"));
    ui_match_case_checkbox->setText (tr ("Match case"));
    ui_replace_label->setText (tr ("Replace with") + ":");
    ui_regex_search_checkbox->setText (tr ("Regex search"));
    ui_whole_words_checkbox->setText (tr ("Whole words only"));

    //
    // Arrange the widgets in a nice layout...
    //
    ui_layout->setSpacing (10);
    ui_layout->addWidget (ui_find_label,              1, 1, 1, 1);
    ui_layout->addWidget (ui_find_lineedit,           1, 2, 1, 2);
    ui_layout->addWidget (ui_replace_label,           2, 1, 2, 1);
    ui_layout->addWidget (ui_next_button,             1, 4, 1, 4);
    ui_layout->addWidget (ui_replace_button,          2, 4, 2, 4);
    ui_layout->addWidget (ui_replace_lineedit,        2, 2, 2, 2);
    ui_layout->addWidget (ui_replace_all_button,      4, 4, 3, 4);
    ui_layout->addWidget (ui_match_case_checkbox,     4, 1, 3, 1);
    ui_layout->addWidget (ui_whole_words_checkbox,    8, 1, 3, 1);
    ui_layout->addWidget (ui_regex_search_checkbox,   6, 1, 3, 1);
    ui_layout->addWidget (ui_done_button,             8, 4, 4, 4);

    //
    // Inhibit resizing
    //
    layout()->setSizeConstraint (QLayout::SetFixedSize);

    //
    // Disable replace functions
    //
    ui_replace_button->setEnabled (false);
    ui_replace_lineedit->setEnabled (false);
    ui_replace_all_button->setEnabled (false);

    //
    // Connect the UI with the dialog logic
    //
    connect (ui_done_button, SIGNAL (clicked()), this, SLOT (hide()));
    connect (ui_next_button, SIGNAL (clicked()), this, SLOT (findNext()));
    connect (ui_match_case_checkbox, SIGNAL (clicked()), this, SLOT (search()));
    connect (ui_whole_words_checkbox, SIGNAL (clicked()), this, SLOT (search()));
    connect (ui_regex_search_checkbox, SIGNAL (clicked()), this, SLOT (search()));
    connect (ui_replace_all_button, SIGNAL (clicked()), this, SLOT (replaceAll()));
    connect (ui_find_lineedit, SIGNAL (textChanged (QString)), this, SLOT (search()));
    connect (ui_replace_button, SIGNAL (clicked()), this, SLOT (replaceFirstOccurrence()));
}

/*!
 * Searches the text editor with the given options by the user and
 * adapts the UI interface based on the search results
 */

bool SearchDialog::search (void) {
    bool _found = m_text_edit->findFirst (ui_find_lineedit->text(),               // Search query
                                          ui_regex_search_checkbox->isChecked(),  // Regex searching
                                          ui_match_case_checkbox->isChecked(),    // Match case
                                          ui_whole_words_checkbox->isChecked(),   // Find whole words
                                          true                                    // Search with word-wrap
                                         );

    ui_replace_button->setEnabled (_found);
    ui_replace_lineedit->setEnabled (_found);
    ui_replace_all_button->setEnabled (_found);

    return _found;
}

/*!
 * Highlights the next match of the search query
 */

void SearchDialog::findNext (void) {
    m_text_edit->findNext();
}

/*!
 * Replaces all matches of the search query
 */

void SearchDialog::replaceAll (void) {
    if (!m_text_edit->isReadOnly()) {
        while (search())
            replaceFirstOccurrence();

        QMessageBox::information (this, windowTitle(),
                                  tr ("All instances of \"%1\" have been replaced with \"%2\"")
                                  .arg (ui_find_lineedit->text())
                                  .arg (ui_replace_lineedit->text()));
    }

    else
        QMessageBox::warning (this, tr ("Find/Replace"), tr ("Document is read-only!"));
}

/*!
 * Replaces the selected match of the search query
 */

void SearchDialog::replaceFirstOccurrence (void) {
    if (!m_text_edit->isReadOnly()) {
        m_text_edit->replaceSelectedText (ui_replace_lineedit->text());
        search();
    }

    else
        QMessageBox::warning (this, tr ("Find/Replace"), tr ("Document is read-only!"));
}
