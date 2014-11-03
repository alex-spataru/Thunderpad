//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#ifndef THEME_H
#define THEME_H

#include <qfile.h>
#include <qcolor.h>
#include <qobject.h>
#include <qsettings.h>

#include <QXmlStreamReader>

/*!
 * \class Theme
 *
 * The \c Theme class is in charge of reading the specified colors
 * of an inputed XML theme and returning the individual values of
 * each color used to theme the \c Editor widget.
 */

class Theme : public QObject
{
        Q_OBJECT

    public:
        explicit Theme (QObject *parent = 0);

        public
    slots:
        void readTheme (const QString &theme);

        QColor background();
        QColor foreground();
        QColor highlightBackground();
        QColor highlightForeground();
        QColor currentLineBackground();
        QColor lineNumbersBackground();
        QColor lineNumbersForeground();

        QColor others();
        QColor numbers();
        QColor strings();
        QColor keywords();
        QColor comments();
        QColor functions();
        QColor data_types();

    private:
        QString m_background;
        QString m_foreground;
        QString m_highlight_background;
        QString m_highlight_foreground;
        QString m_current_line_background;
        QString m_line_numbers_background;
        QString m_line_numbers_foreground;

        QString m_others;
        QString m_numbers;
        QString m_strings;
        QString m_keywords;
        QString m_comments;
        QString m_functions;
        QString m_data_types;

        void useFallbackColors();
};

#endif  // THEME_H
