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

#include <QLabel>
#include <QString>
#include <QRegExp>
#include <QSettings>

#include "editor.h"
#include "window.h"
#include "defaults.h"
#include "statusbar.h"

/*!
 * \class StatusBar
 * The \c StatusBar class is in charge of creating and configuring
 * a new QStatusBar widget to be used with the \c Window class.
 */

 /*!
  * \internal
  * Initializes and configures the status bar
  */

StatusBar::StatusBar (Window *parent) : QStatusBar (parent) {
    updateSettings();
    initialize (parent);
}

/*!
 * \internal
 * Creates the statusbar widgets and connects them to the events and signals
 * of the text editor.
 */

void StatusBar::initialize (Window *window) {
    Q_ASSERT (window != NULL);

    window->setStatusBar (this);
    m_text_edit = window->editor();

    m_size_label = new QLabel (this);
    m_lines_label = new QLabel (this);
    m_words_label = new QLabel (this);

    addPermanentWidget (m_size_label);
    addPermanentWidget (m_lines_label);
    addPermanentWidget (m_words_label);

    connect (m_text_edit, SIGNAL (textChanged()), this, SLOT (updateStatusLabel()));
    connect (window, SIGNAL (updateSettings()), this, SLOT (updateSettings()));

    updateStatusLabel();
}

/*!
 * \internal
 * Hides or shows the statusbar based on the current settings of the application
 */

void StatusBar::updateSettings (void) {
    settings()->value ("statusbar-enabled", SETTINGS_STATUSBAR_ENABLED).toBool() ? show() : hide();
}

/*!
 * \internal
 * Updates the text of the statusbar widgets
 */

void StatusBar::updateStatusLabel (void) {
    m_size_label->setText ("  " + fileSize() + "  ");
    m_lines_label->setText ("  " + lineCount() + "  ");
    m_words_label->setText ("  " + wordCount() + "  ");
}

/*!
 * \internal
 * Returns the lenght of the text editor in bytes, KB and MB
 */

QString StatusBar::fileSize (void) {
    return tr ("Size:") + " " +
           m_text_edit->calculateSize();
}

/*!
 * \internal
 * Returns the number of words in the text editor
 */

QString StatusBar::wordCount (void) {
    return tr ("Words:") + " " +
           QString::number (m_text_edit->wordCount());
}

/*!
* \internal
* Returns the number of lines of the text editor document
*/

QString StatusBar::lineCount (void) {
    return tr ("Lines:") + " " +
           QString::number (m_text_edit->lines());
}

/*!
 * Allows the class to access the application settings
 */

QSettings *StatusBar::settings (void) const {
    return new QSettings (APP_COMPANY, APP_NAME);
}

