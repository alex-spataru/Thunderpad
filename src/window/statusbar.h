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

#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QLabel>
#include <QString>
#include <QRegExp>
#include <QSettings>
#include <QStatusBar>

#include "editor.h"
#include "window.h"

/*!
 * \class StatusBar
 *
 * The \c StatusBar class is in charge of creating and configuring
 * a new QStatusBar widget to be used with the \c Window class.
 */

class StatusBar : public QStatusBar {
        Q_OBJECT

    public:
        StatusBar (Window *parent = 0);

    private slots:
        void updateSettings (void);
        void updateStatusLabel (void);
        void initialize (Window *window);

    private:
        QLabel *m_size_label;
        QLabel *m_words_label;
        QLabel *m_lines_label;
        QSettings *m_settings;
        Editor *m_text_edit;

        QString fileSize (void);
        QString wordCount (void);
        QString lineCount (void);
};

#endif
