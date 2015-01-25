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

#include <QFile>
#include <QXmlStreamReader>

#include "theme.h"
#include "assembly_info.h"

#define COLOR_SCHEMES_PATH ":/color-schemes/"

/*!
 * \class Theme
 * \brief Loads theme definitions into the text editor
 *
 * The \c Theme class is in charge of reading the specified colors
 * of an inputed XML theme and returning the individual values of
 * each color used to theme the \c Editor widget.
 */


/*! \internal
 * Initializes the class by doing nothing
 */

Theme::Theme (QObject *parent) : QObject (parent)
{
}

/*!
 * Applies the fallback colors.
 * Its only called in the case that a color theme
 * cannot be loaded during run-time.
 */

void Theme::useFallbackColors (void)
{
    m_background = "#ffffff";
    m_foreground = "#555555";
    m_highlight_background = "#b0c4dc";
    m_highlight_foreground = "#555555";
    m_line_numbers_background = "#f0f0f0";
    m_line_numbers_foreground = "#a8a8a8";
    m_current_line_background = "#ffffd1";
}

/*!
 * Reads data values from the specified \a {theme} definition
 * and loads them into the program.
 */

void Theme::readTheme (const QString &theme)
{
    Q_ASSERT (!theme.isEmpty());
    QFile _file (COLOR_SCHEMES_PATH + theme + ".xml");

    if (!_file.open (QFile::ReadOnly))
    {
        useFallbackColors();
        return;
    }

    QStringList _types;
    QStringList _values;

    QXmlStreamReader *_xml_reader = new QXmlStreamReader (&_file);

    while (!_xml_reader->atEnd() && !_xml_reader->hasError())
    {
        QXmlStreamReader::TokenType token = _xml_reader->readNext();

        if (token == QXmlStreamReader::StartElement)
        {
            if (_xml_reader->name() == "type")
                _types.append (_xml_reader->readElementText());

            if (_xml_reader->name() == "color")
                _values.append (_xml_reader->readElementText());
        }
    }

    _xml_reader->clear();
    _file.close();

    if (_types.count() != _values.count())
    {
        useFallbackColors();
        return;
    }

    else
    {
        m_background = _values.at (_types.indexOf ("background"));
        m_foreground = _values.at (_types.indexOf ("foreground"));
        m_highlight_background = _values.at (_types.indexOf ("highlight_background"));
        m_highlight_foreground = _values.at (_types.indexOf ("highlight_foreground"));
        m_line_numbers_background = _values.at (_types.indexOf ("line_numbers_background"));
        m_line_numbers_foreground = _values.at (_types.indexOf ("line_numbers_foreground"));
        m_current_line_background = _values.at (_types.indexOf ("current_line_background"));

        m_others = _values.at (_types.indexOf ("others"));
        m_numbers = _values.at (_types.indexOf ("numbers"));
        m_strings = _values.at (_types.indexOf ("strings"));
        m_keywords = _values.at (_types.indexOf ("keywords"));
        m_comments = _values.at (_types.indexOf ("comments"));
        m_functions = _values.at (_types.indexOf ("functions"));
    }
}

/* General text editor colors */

/*!
 * The background color of the text editor
 */

QColor Theme::background (void) const
{
    return QColor (m_background);
}

/*!
 * The foreground color of the text editor
 */

QColor Theme::foreground (void) const
{
    return QColor (m_foreground);
}

/*!
 * The highlight background color of the text editor
 */

QColor Theme::highlightBackground (void) const
{
    return QColor (m_highlight_background);
}

/*!
 * The highlight foreground color of the text editor
 */

QColor Theme::highlightForeground (void) const
{
    return QColor (m_highlight_foreground);
}

/*!
 * The caret line background color of the text editor
 */

QColor Theme::currentLineBackground (void) const
{
    return QColor (m_current_line_background);
}

/*!
 * The line numbers margin background color of the text editor
 */

QColor Theme::lineNumbersBackground (void) const
{
    return QColor (m_line_numbers_background);
}

/*!
 * The line numbers margin foreground color of the text editor
 */

QColor Theme::lineNumbersForeground (void) const
{
    return QColor (m_line_numbers_foreground);
}

/* Syntax highlighter colors */

/*!
 * The color used to highlight other shit in the text editor
 */

QColor Theme::others (void) const
{
    return QColor (m_others);
}

/*!
 * The color used to highlight numbers in the text editor
 */

QColor Theme::numbers (void) const
{
    return QColor (m_numbers);
}

/*!
 * The color used to highlight strings in the text editor
 */

QColor Theme::strings (void) const
{
    return QColor (m_strings);
}

/*!
 * The color used to highlight keywords in the text editor
 */

QColor Theme::keywords (void) const
{
    return QColor (m_keywords);
}

/*!
 * The color used to highlight comments in the text editor
 */

QColor Theme::comments (void) const
{
    return QColor (m_comments);
}

/*!
 * The color used to highlight functions in the text editor
 */

QColor Theme::functions (void) const
{
    return QColor (m_functions);
}
