//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#include "syntax-highlighter.h"

/*!
 * \brief SyntaxHighlighter::SyntaxHighlighter
 * \param parent
 */

SyntaxHighlighter::SyntaxHighlighter (QTextDocument *parent)
    : QSyntaxHighlighter (parent) {}

/*!
 * \brief SyntaxHighlighter::setLanguage
 * \param lang the language to apply
 *
 * Changes the current programming language and rehighlights
 * the document.
 *
 * The function opens an XML file (based on the \c lang parementer)
 * that contains information about the selected programming language, such
 * as the language's keywords, functions, variables, constants, etc..
 */

void SyntaxHighlighter::setLanguage (const QString &lang)
{
    Q_ASSERT (!lang.isEmpty());
    Q_UNUSED (lang);
}

/*!
 * \brief SyntaxHighlighter::updateColor
 * \param theme
 *
 * Updates the colors that are used to highlight the document
 */

void SyntaxHighlighter::updateColor (Theme *theme)
{
    Q_ASSERT (!theme == NULL);

    m_othersColor = theme->others();
    m_functionsColor = theme->functions();
    m_commentsColor = theme->comments();
    m_keyworsColor = theme->keywords();
    m_numbersColor = theme->numbers();
    m_stringColor = theme->strings();
    m_typesColor = theme->data_types();
    rehighlight();
}

/*!
 * \brief SyntaxHighlighter::highlightBlock
 * \param text the text to process and highlight
 *
 * This function is used everytime when the text of the document changes.
 * Instead of rehighlighting all the document when the user tipes something,
 * the function only updates and highlights the new "chunks" of the text
 *document.
 * If you need to highlight all the document, use the rehighlight() function or
 *input
 * all the document's text to the \c text parameter of this function.
 */

void SyntaxHighlighter::highlightBlock (const QString &text)
{
    Q_UNUSED (text);
}
