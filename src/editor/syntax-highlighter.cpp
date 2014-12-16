//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#include "syntax-highlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent) {}

void SyntaxHighlighter::setLanguage(const QString &lang) {
    Q_ASSERT(!lang.isEmpty());
    Q_UNUSED(lang);
}

void SyntaxHighlighter::updateColor(Theme *theme) {
    Q_ASSERT(!theme == NULL);

    m_othersColor = theme->others();
    m_functionsColor = theme->functions();
    m_commentsColor = theme->comments();
    m_keyworsColor = theme->keywords();
    m_numbersColor = theme->numbers();
    m_stringColor = theme->strings();
    m_typesColor = theme->data_types();
    rehighlight();
}

void SyntaxHighlighter::highlightBlock(const QString &text) {
    Q_UNUSED(text);
}


