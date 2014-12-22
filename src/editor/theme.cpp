//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#include "theme.h"

Theme::Theme(QObject *parent) : QObject(parent) {
    QSettings _settings(APP_COMPANY, APP_NAME);
}

void Theme::useFallbackColors(void) {
    qDebug() << this << "Error loading theme, using fallback colors";

    m_background = "#ffffff";
    m_foreground = "#555555";
    m_highlight_background = "#b0c4dc";
    m_highlight_foreground = "#555555";
    m_line_numbers_background = "#f0f0f0";
    m_line_numbers_foreground = "#a8a8a8";
    m_current_line_background = "#ffffd1";
}

void Theme::readTheme(const QString &theme) {
    Q_ASSERT(!theme.isEmpty());
    qDebug() << this << "Loading color scheme with name" << theme << "";

    QFile _file(":/color-schemes/" + theme + ".xml");

    if (!_file.open(QFile::ReadOnly)) {
        useFallbackColors();
        return;
    }

    QStringList _types;
    QStringList _values;

    QXmlStreamReader *_xml_reader = new QXmlStreamReader(&_file);

    while (!_xml_reader->atEnd() && !_xml_reader->hasError()) {
        QXmlStreamReader::TokenType token = _xml_reader->readNext();

        if (token == QXmlStreamReader::StartElement) {
            if (_xml_reader->name() == "type")
                _types.append(_xml_reader->readElementText());

            if (_xml_reader->name() == "color")
                _values.append(_xml_reader->readElementText());
        }
    }

    _xml_reader->clear();
    _file.close();

    if (_types.count() != _values.count()) {
        useFallbackColors();
        return;
    }

    else {
        m_background = _values.at(_types.indexOf("background"));
        m_foreground = _values.at(_types.indexOf("foreground"));
        m_highlight_background = _values.at(_types.indexOf("highlight_background"));
        m_highlight_foreground = _values.at(_types.indexOf("highlight_foreground"));
        m_line_numbers_background = _values.at(_types.indexOf("line_numbers_background"));
        m_line_numbers_foreground = _values.at(_types.indexOf("line_numbers_foreground"));
        m_current_line_background = _values.at(_types.indexOf("current_line_background"));

        m_others = _values.at(_types.indexOf("others"));
        m_keywords = _values.at(_types.indexOf("keywords"));
        m_comments = _values.at(_types.indexOf("comments"));
        m_functions = _values.at(_types.indexOf("functions"));

        qDebug() << this << "Color scheme" << theme << "loaded successfully";
    }
}

QColor Theme::background(void) const {
    return QColor(m_background);
}

QColor Theme::foreground(void) const {
    return QColor(m_foreground);
}

QColor Theme::highlightBackground(void) const {
    return QColor(m_highlight_background);
}

QColor Theme::highlightForeground(void) const {
    return QColor(m_highlight_foreground);
}

QColor Theme::currentLineBackground(void) const {
    return QColor(m_current_line_background);
}

QColor Theme::lineNumbersBackground(void) const {
    return QColor(m_line_numbers_background);
}

QColor Theme::lineNumbersForeground(void) const {
    return QColor(m_line_numbers_foreground);
}

QColor Theme::others(void) const {
    return QColor(m_others);
}

QColor Theme::keywords(void) const {
    return QColor(m_keywords);
}

QColor Theme::comments(void) const {
    return QColor(m_comments);
}

QColor Theme::functions(void) const {
    return QColor(m_functions);
}
