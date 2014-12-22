//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#include "syntax-highlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent) {

}

void SyntaxHighlighter::setLanguage(const QString &lang) {
    m_lang = lang;

    if (m_lang != tr("Plain text") && !m_lang.isEmpty()) {
        // TODO
        qCritical() << this << "function not implemeted yet";
    }

    rehighlight();
}

void SyntaxHighlighter::detectLanguage(const QString &file) {
    Q_ASSERT(!file.isEmpty());
    Q_UNUSED(file);

    // TODO
    qCritical() << this << "function not implemeted yet";
}

void SyntaxHighlighter::updateColor(Theme *theme) {
    Q_ASSERT(theme != NULL);

    m_others_format.setForeground(theme->others());
    m_comments_format.setForeground(theme->comments());
    m_keywords_format.setForeground(theme->keywords());
    m_functions_format.setForeground(theme->functions());

    setLanguage(m_lang);
}

void SyntaxHighlighter::highlightBlock(const QString &text) {
    foreach (const HighlightingRule &rule, m_highlighting_rules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);

        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);
}
