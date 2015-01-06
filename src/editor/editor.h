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

#ifndef EDITOR_H
#define EDITOR_H

#include <QIcon>
#include <QFile>
#include <QPrinter>
#include <QPainter>
#include <QSettings>
#include <QTextBlock>
#include <QFontDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QPrintDialog>
#include <QApplication>
#include <QInputDialog>
#include <QPlainTextEdit>

#include "theme.h"
#include "platform.h"
#include "line-numbers.h"
#include "spell-checker.h"
#include "assembly_info.h"
#include "syntax-highlighter.h"

/*!
 * \class Editor
 *
 * The \c Editor class implements a simple code editor widget that
 * can be customized using XML files. For example, the text editor
 * can load a syntax highlighting definition using a predefined XML
 * file.
 * The widget also supports custom color schemes,
 * which are also defined using XML files.
 *
 * As a final note, the \c Editor handles most of the common functions of a text
 *editing
 * application, such as opening, reading, writting, exporting and printing
 * files, simplyfing its implementation by a great deal.
 */

class Editor : public QPlainTextEdit
{

        Q_OBJECT

    public:
        explicit Editor (QWidget *parent = 0);

        /*!
         * \brief maybeSave
         * \return
         *
         * Returns \c true when the document was:
         *       - saved automatically
         *       - saved by the user
         *       - discarded by the user
         *
         * Returns \c false when the user wants to continue editing the document.
         * (eg. when clicking the "cancel" button)
         */

        bool maybeSave (void);

        /* The following functions are used by the  line numbers widget */
        /* These functions are for internal use only */
        int lineNumberAreaWidth (void);
        void lineNumberAreaPaintEvent (QPaintEvent *event);

    signals:

        /*!
         * \brief updateTitle
         *
         * This signal es emitted when the document title has
         * changed, so that we can change the title of the main window.
         */

        void updateTitle (void);

        /*!
         * \brief settingsChanged
         *
         * This signal es emitted when the user changes a setting of
         * the editor. It allows us to apply the changed settings along
         * all the opened windows/instances of the application.
         */

        void settingsChanged (void);

    public slots:

        /*!
         * \brief checkSpelling
         *
         * Checks the spelling of the document and highlights any errors.
         */

        void checkSpelling (void);

        /*!
         * \brief updateSettings
         *
         * Scans for changes in the user settings and re-loads the widget
         */

        void updateSettings (void);

        /*!
         * \brief save
         * \return
         *
         * Saves the current document and returns \c true if the
         * saving is successfull. In the case that the document has not
         * been saved to the hard disk storage (eg: editing an untitled document),
         * this function calls the \c saveAs() function.
         */

        bool save (void);

        /*!
         * \brief saveAs
         * \return
         *
         * Shows a file dialog and saves the current document with the given user
         *input.
         */

        bool saveAs (void);

        /*!
         * \brief goToLine
         *
         * Navigates to the given line number by prompting the user with a dialog
         */

        void goToLine (void);

        /*!
         * \brief sortSelection
         *
         * Sorts the current selection by alphabetical and numerical order
         */

        void sortSelection (void);

        /*!
         * \brief insertDateTime
         *
         * Inserts the current date and time in the cursor
         */

        void insertDateTime (void);

        /*!
         * \brief print
         *
         * Shows a print dialog that allows the user to send the current
         * document to the connected printer(s).
         */

        void print (void);

        /*!
         * \brief exportPdf
         *
         * Shows a file dialog and exports the current document as a PDF
         * file given the user input.
         */

        void exportPdf (void);

        /*!
         * \brief exportHtml
         *
         * Shows a file dialog and exports the current document as a HTML
         * file given the user input.
         */

        void exportHtml (void);

        /*!
         * \brief selectFonts
         *
         * Shows a font dialog and changes the editor's font based on
         * the selected user option.
         */

        void selectFonts (void);

        /*!
         * \brief setWordWrap
         * \param ww
         *
         * Enables or disables word wrapping based on the \c ww parameter
         */

        void setWordWrap (bool ww);

        /*!
         * \brief readFile
         * \param file
         *
         * Loads the given file into the editor.
         */

        void readFile (const QString &file);

        /*!
         * \brief writeFile
         * \param file
         * \return
         *
         * Writes the content of the text document in the given file
         */

        bool writeFile (const QString &file);

        /*!
         * \brief setSyntaxLanguage
         * \param language
         *
         * Highlights the document based on the given programming
         * language.
         */

        void setSyntaxLanguage (const QString &language);

    protected:
        void resizeEvent (QResizeEvent *event);

    private slots:
        void highlightCurrentLine (void);
        void configureDocument (const QString &file);
        void updateLineNumberArea (const QRect &, int);
        void updateLineNumberAreaWidth (int newBlockCount);

    private:
        Theme *m_theme;
        QSettings *m_settings;
        QWidget *m_lineNumberArea;
        SyntaxHighlighter *m_highlighter;

        bool _hc_line_enabled;
};

#endif
