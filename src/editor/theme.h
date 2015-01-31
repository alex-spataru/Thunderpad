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

#include <QColor>

class Theme : public QObject {
        Q_OBJECT

    public:
        explicit Theme (QObject *parent = 0);

        QColor background (void) const;
        QColor foreground (void) const;
        QColor highlightBackground (void) const;
        QColor highlightForeground (void) const;
        QColor currentLineBackground (void) const;
        QColor lineNumbersBackground (void) const;
        QColor lineNumbersForeground (void) const;

        QColor others (void) const;
        QColor numbers (void) const;
        QColor strings (void) const;
        QColor keywords (void) const;
        QColor comments (void) const;
        QColor functions (void) const;

    public slots:
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
        QString m_strings;
        QString m_numbers;
        QString m_keywords;
        QString m_comments;
        QString m_functions;

        void useFallbackColors (void);
};

#endif
