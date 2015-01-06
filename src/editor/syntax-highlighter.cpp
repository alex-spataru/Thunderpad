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

#include "syntax-highlighter.h"

SyntaxHighlighter::SyntaxHighlighter (QTextDocument *parent)
    : QSyntaxHighlighter (parent)
{

}

void SyntaxHighlighter::setLanguage (const QString &lang)
{
    m_lang = lang;

    if (m_lang != tr ("Plain text") && !m_lang.isEmpty())
        {
        // TODO
        }

    rehighlight();
}

void SyntaxHighlighter::detectLanguage (const QString &file)
{
    Q_ASSERT (!file.isEmpty());
    Q_UNUSED (file);

    // TODO
}

void SyntaxHighlighter::updateColor (Theme *theme)
{
    Q_ASSERT (theme != NULL);

    m_others_format.setForeground (theme->others());
    m_comments_format.setForeground (theme->comments());
    m_keywords_format.setForeground (theme->keywords());
    m_functions_format.setForeground (theme->functions());

    setLanguage (m_lang);
}

void SyntaxHighlighter::highlightBlock (const QString &text)
{
    foreach (const HighlightingRule &rule, m_highlighting_rules)
        {
        QRegExp expression (rule.pattern);
        int index = expression.indexIn (text);

        while (index >= 0)
            {
            int length = expression.matchedLength();
            setFormat (index, length, rule.format);
            index = expression.indexIn (text, index + length);
            }
        }

    setCurrentBlockState (0);
}
