//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#ifndef EDITOR_H
#define EDITOR_H

#include <qicon.h>
#include <qfile.h>
#include <qpainter.h>
#include <qsettings.h>
#include <qfontdialog.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qapplication.h>
#include <qplaintextedit.h>

#include <QTextBlock>

#include "theme.h"
#include "platform.h"
#include "line-numbers.h"
#include "syntax-highlighter.h"

/*!
 * \class Editor
 *
 * The \c Editor class implements a simple code editor widget that
 * can be customized using XML files. For example, the text editor
 * can load a syntax highlighting definition using a predefined XML
 * file and can also be themed using XML files.
 *
 * The \c Editor handles most of the common functions of a text editing
 * application, such as opening, reading, writting, exporting and printing
 * files.
 */

class Editor : public QPlainTextEdit
{
        Q_OBJECT

    public:
        explicit Editor (QWidget *parent = 0);
        bool maybeSave();

        void lineNumberAreaPaintEvent (QPaintEvent *event);
        int lineNumberAreaWidth();

    protected:
        void resizeEvent (QResizeEvent *event);

        private
    slots:
        void highlightCurrentLine();
        void updateLineNumberAreaWidth (int newBlockCount);
        void updateLineNumberArea (const QRect &, int);

    signals:
        /// This signal is emitted when the title of the document changes
        void updateTitle();

        /// This signal is emitted when the user changes a feature of the text
        /// editor, such as the default font used for editing.
        void settingsChanged();

        public
    slots:
        void updateSettings();

        bool save();
        bool saveAs();

        void print();
        void exportPdf();
        void exportHtml();
        void selectFonts();
        void setReadOnly (bool ro);
        void setWordWrap (bool ww);
        int find (const QString &query);
        void replace (const QString &query);
        void replaceAll (const QString &query);
        void readFile (const QString &file);
        bool writeFile (const QString &file);
        void setSyntaxLanguage (const QString &language);

    private:
        Theme *m_theme;
        QWidget *m_lineNumberArea;
        SyntaxHighlighter *m_highlighter;

        bool _hc_line_enabled;
};

#endif
