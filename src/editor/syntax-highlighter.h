//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#ifndef SYNTAX_HIGHLIGHTER_H
#define SYNTAX_HIGHLIGHTER_H

#include <QRegExp>
#include <QStringList>
#include <QTextStream>
#include <QTextCharFormat>
#include <QXmlStreamReader>
#include <QSyntaxHighlighter>

#include "theme.h"

/*!
 * \class SyntaxHighlighter
 *
 * The \c SyntaxHighlighter is in charge of highlighting the contents
 * of the document of the \c Editor and highlighting relevant matches,
 * such as keywords, types and comments.
 *
 * The \c SyntaxHighlighter uses the highlighting definitions specified
 * by the XML files found in the resources folder of Thunderpad.
 */

class SyntaxHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    explicit SyntaxHighlighter(QTextDocument *parent);

public slots:
    void updateColor(Theme *theme);
    void setLanguage(const QString &lang);

protected:
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QString _others;
    QString _functions;
    QString _comment_end;
    QString _comment_start;
    QString _multiline_comment_end;
    QString _multiline_comment_start;

    QStringList _keywords;
    QStringList _data_types;

    QColor m_othersColor;
    QColor m_functionsColor;
    QColor m_commentsColor;
    QColor m_keyworsColor;
    QColor m_numbersColor;
    QColor m_stringColor;
    QColor m_typesColor;
};

#endif
