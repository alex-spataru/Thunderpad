//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#include "theme.h"

Theme::Theme (QObject *parent) : QObject (parent)
{
    QSettings _settings ("Alex Spataru", "Thunderpad");
    readTheme (_settings.value ("color-scheme", "Light").toString());
}

void Theme::useFallbackColors()
{
    m_background = "#fff";
    m_foreground = "#555";
    m_highlight_background = "#b0c4dc";
    m_highlight_foreground = "#555";
    m_line_numbers_background = "#fff";
    m_line_numbers_foreground = "#a8a8a8";
    m_current_line_background = "#ffffd1";
}

void Theme::readTheme (const QString &theme)
{
    Q_ASSERT (!theme.isEmpty());

    QFile _file (":/color-schemes/" + theme + ".xml");

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
        m_line_numbers_background =
            _values.at (_types.indexOf ("line_numbers_background"));
        m_line_numbers_foreground =
            _values.at (_types.indexOf ("line_numbers_foreground"));
        m_current_line_background =
            _values.at (_types.indexOf ("current_line_background"));

        m_others = _values.at (_types.indexOf ("others"));
        m_numbers = _values.at (_types.indexOf ("numbers"));
        m_strings = _values.at (_types.indexOf ("strings"));
        m_keywords = _values.at (_types.indexOf ("keywords"));
        m_comments = _values.at (_types.indexOf ("comments"));
        m_functions = _values.at (_types.indexOf ("functions"));
        m_data_types = _values.at (_types.indexOf ("data_types"));
    }
}

QColor Theme::background() const
{
    return QColor (m_background);
}

QColor Theme::foreground() const
{
    return QColor (m_foreground);
}

QColor Theme::highlightBackground() const
{
    return QColor (m_highlight_background);
}

QColor Theme::highlightForeground() const
{
    return QColor (m_highlight_foreground);
}

QColor Theme::currentLineBackground() const
{
    return QColor (m_current_line_background);
}

QColor Theme::lineNumbersBackground() const
{
    return QColor (m_line_numbers_background);
}

QColor Theme::lineNumbersForeground() const
{
    return QColor (m_line_numbers_foreground);
}

QColor Theme::others() const
{
    return QColor (m_others);
}

QColor Theme::numbers() const
{
    return QColor (m_numbers);
}

QColor Theme::strings() const
{
    return QColor (m_strings);
}

QColor Theme::keywords() const
{
    return QColor (m_keywords);
}

QColor Theme::comments() const
{
    return QColor (m_comments);
}

QColor Theme::functions() const
{
    return QColor (m_functions);
}

QColor Theme::data_types() const
{
    return QColor (m_data_types);
}
