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

#ifndef THEME_H
#define THEME_H

#include <QFile>
#include <QColor>
#include <QObject>
#include <QSettings>
#include <QXmlStreamReader>

#include "assembly_info.h"

/*!
 * \class Theme
 *
 * The \c Theme class is in charge of reading the specified colors
 * of an inputed XML theme and returning the individual values of
 * each color used to theme the \c Editor widget.
 */

class Theme : public QObject {
        Q_OBJECT

    public:
        explicit Theme (QObject *parent = 0);

        /* The following colors are used by the text editor */

        /*!
         * \brief background
         * \return
         *
         * Returns the background color of the text editor
         */

        QColor background (void) const;

        /*!
         * \brief foreground
         * \return
         *
         * Returns the foreground color of the text editor
         */

        QColor foreground (void) const;

        /*!
         * \brief highlightBackground
         * \return
         *
         * Returns the background color of highlighted text
         */

        QColor highlightBackground (void) const;

        /*!
         * \brief highlightForeground
         * \return
         *
         * Returns the foreround color of highlighted text
         */

        QColor highlightForeground (void) const;

        /*!
         * \brief currentLineBackground
         * \return
         *
         * Returns the background color of the highlighted current line
         */

        QColor currentLineBackground (void) const;

        /*!
         * \brief lineNumbersBackground
         * \return
         *
         * Returns the background color of the line numbers area
         */

        QColor lineNumbersBackground (void) const;

        /*!
         * \brief lineNumbersForeground
         * \return
         *
         * Returns the foreground color of the line numbers area
         */

        QColor lineNumbersForeground (void) const;

        /* The following colors are used by the syntax highlighter */

        /*!
         * \brief others
         * \return
         *
         * Returns the color used to highlight other data types
         * in the syntax highlighter.
         */

        QColor others (void) const;

        /*!
         * \brief keywords
         * \return
         *
         * Returns the color used to highlight keywords
         * in the syntax highlighter.
         */

        QColor keywords (void) const;

        /*!
         * \brief comments
         * \return
         *
         * Returns the color used to highlight comments
         * in the syntax highlighter.
         */

        QColor comments (void) const;

        /*!
         * \brief functions
         * \return
         *
         * Returns the color used to highlight functions
         * in the syntax highlighter.
         */

        QColor functions (void) const;

    public slots:

        /*!
         * \brief readTheme
         * \param theme
         *
         * Reads and configures the given theme by
         * loading its respective XML definition
         */

        void readTheme (const QString &theme);

    private:
        QString m_background;
        QString m_foreground;
        QString m_highlight_background;
        QString m_highlight_foreground;
        QString m_current_line_background;
        QString m_line_numbers_background;
        QString m_line_numbers_foreground;

        QString m_others;
        QString m_keywords;
        QString m_comments;
        QString m_functions;
        QString m_strings;
        QString m_numbers;

        void useFallbackColors (void);
};

#endif
