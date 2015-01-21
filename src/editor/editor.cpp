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

#include <QIcon>
#include <QMessageBox>
#include <QFileDialog>
#include <QFontDialog>
#include <QApplication>
#include <QPrintDialog>

#include <Qsci/qsciprinter.h>
#include <Qsci/qscilexer.h>
#include <Qsci/qscilexeravs.h>
#include <Qsci/qscilexerbash.h>
#include <Qsci/qscilexerbatch.h>
#include <Qsci/qscilexercmake.h>
#include <Qsci/qscilexercoffeescript.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexercsharp.h>
#include <Qsci/qscilexercss.h>
#include <Qsci/qscilexerd.h>
#include <Qsci/qscilexerdiff.h>
#include <Qsci/qscilexerfortran.h>
#include <Qsci/qscilexerfortran77.h>
#include <Qsci/qscilexerhtml.h>
#include <Qsci/qscilexeridl.h>
#include <Qsci/qscilexerjava.h>
#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qscilexerlua.h>
#include <Qsci/qscilexermakefile.h>
#include <Qsci/qscilexermatlab.h>
#include <Qsci/qscilexeroctave.h>
#include <Qsci/qscilexerpascal.h>
#include <Qsci/qscilexerperl.h>
#include <Qsci/qscilexerpo.h>
#include <Qsci/qscilexerpostscript.h>
#include <Qsci/qscilexerpov.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexerruby.h>
#include <Qsci/qscilexerspice.h>
#include <Qsci/qscilexersql.h>
#include <Qsci/qscilexertcl.h>
#include <Qsci/qscilexertex.h>
#include <Qsci/qscilexerverilog.h>
#include <Qsci/qscilexervhdl.h>
#include <Qsci/qscilexerxml.h>
#include <Qsci/qscilexeryaml.h>

#include "editor.h"
#include "platform.h"
#include "assembly_info.h"

Editor::Editor (QWidget *parent) : QsciScintilla (parent)
{
    setAttribute (Qt::WA_DeleteOnClose);

    m_theme = new Theme (this);
    m_settings = new QSettings (APP_COMPANY, APP_NAME);

    setIndentationWidth(4);
    setIndentationGuides(true);
    setFolding(QsciScintilla::BoxedTreeFoldStyle, 2);
    setBraceMatching (QsciScintilla::SloppyBraceMatch);

    connect (this, SIGNAL (textChanged()), this, SLOT (updateLineNumbers()));
    connect (this, SIGNAL (settingsChanged()), this, SLOT (updateSettings()));

    m_current_language = PlainText;
}

bool Editor::maybeSave (void)
{
    // We don't need to save the document if it isn't modified
    if (!isModified())
        return true;

    // The document was already saved in the hard disk, however, it has unsaved changes
    if (!documentTitle().isEmpty() && isModified())
        return save();

    // The document was never saved in the hard disk, so ask the user if he/she wants
    // to save the current document
    else if (isModified())
    {
        QMessageBox _message;
        _message.setParent (this);
        _message.setWindowModality (Qt::WindowModal);
        _message.setWindowTitle (tr ("Unsaved changes"));
        _message.setIconPixmap (QPixmap (":/images/others/logo.png"));
        _message.setStandardButtons (QMessageBox::Save | QMessageBox::Cancel |
                                     QMessageBox::Discard);
        _message.setText (
            "<b>" + tr ("This document has changes, do you want to save them?") +
            "</b>");

        _message.setInformativeText (
            tr ("Your changes will be lost if you close this item without saving."));

        switch (_message.exec())
        {
            case QMessageBox::Save:
                return save();
                break;

            case QMessageBox::Discard:
                return true;
                break;

            default:
                return false;
                break;
        }
    }

    return false;
}

QString Editor::documentTitle (void) const
{
    return m_document_title;
}

void Editor::resetZoom (void)
{
    zoomTo(0);
}

void Editor::documentInfo (void)
{
}

void Editor::updateSettings (void)
{
    // Specify default values for the font
    int _default_size = 11;
    QString _default_font = "Courier";

#if MAC_OS_X
    _default_font = "Menlo";
#endif

#if WINDOWS
    _default_size = 10;

    // Use Consolas font if Windows version is Vista or greater
    _default_font = QSysInfo::windowsVersion() >= 0x0080 ? "Consolas" : "Courier New";
#endif

#if LINUX
    // Get system monospaced font?
#endif

    // Load the saved font
    m_font.setBold (m_settings->value ("font-bold", false).toBool());
    m_font.setItalic (m_settings->value ("font-italic", false).toBool());
    m_font.setUnderline (m_settings->value ("font-underline", false).toBool());
    m_font.setPointSize (m_settings->value ("font-size", _default_size).toInt());
    m_font.setFamily (m_settings->value ("font-family", _default_font).toString());

    // Use the same font in the editor
    setFont (m_font);
    setMarginsFont (m_font);

    // Enable/disable word wrapping based on the saved settings
    setWordWrap (m_settings->value ("wordwrap-enabled", true).toBool());

    // Update the colors of the text editor
    m_theme->readTheme (m_settings->value ("color-scheme", "Light").toString());
    setMarginsBackgroundColor (m_theme->lineNumbersBackground());
    setMarginsForegroundColor (m_theme->lineNumbersForeground());
    setCaretLineBackgroundColor (m_theme->currentLineBackground());

    // Enable the caret line (highlight the current line)
    setCaretLineVisible (m_settings->value ("hc-line-enabled", true).toBool());

    m_line_numbers = m_settings->value ("line-numbers-enabled", true).toBool();

    // Enable line numbers
    if (m_line_numbers)
    {
        setMarginLineNumbers (1, true);
        setMarginWidth (1, QString ("00%1").arg (lines()));
    }

    // Disable line numbers
    else
    {
        setMarginWidth (1, 0);
        setMarginLineNumbers (1, false);
    }

    // Basically, re-highlight the document
    configureLexer ();
}

bool Editor::save (void)
{
    return documentTitle().isEmpty() ? saveAs() : writeFile (documentTitle());
}

bool Editor::saveAs (void)
{
    return writeFile (QFileDialog::getSaveFileName (this, tr ("Save as") + "...",
                      QDir::homePath()));
}

void Editor::goToLine (void)
{

}

void Editor::sortSelection (void)
{

}

void Editor::insertDateTime (void)
{
    // Todo
}

void Editor::print (void)
{
    QsciPrinter printer;
    QPrintDialog dialog (&printer, this);

    if (dialog.exec() == QDialog::Accepted)
    {
        printer.setWrapMode (wrapMode());
        printer.printRange (this, 0);
    }
}

void Editor::exportPdf (void)
{
}

void Editor::exportHtml (void)
{

}

void Editor::selectFonts (void)
{
    QFontDialog _dialog;
    _dialog.setCurrentFont (m_font);

    if (_dialog.exec() == QFontDialog::Accepted)
    {
        m_font = _dialog.selectedFont();

        // Fonts cannot be saved directly
        m_settings->setValue ("font-bold", m_font.bold());
        m_settings->setValue ("font-italic", m_font.italic());
        m_settings->setValue ("font-family", m_font.family());
        m_settings->setValue ("font-size", m_font.pointSize());
        m_settings->setValue ("font-underline", m_font.underline());

        emit settingsChanged();
    }
}

void Editor::setWordWrap (bool ww)
{
    setWrapMode (ww ? QsciScintilla::WrapWord : QsciScintilla::WrapNone);
}

void Editor::readFile (const QString &file)
{
    qApp->setOverrideCursor (Qt::WaitCursor);

    if (!file.isEmpty())
    {
        QFile _file (file);

        if (_file.open (QIODevice::ReadOnly))
        {
            setText (QString::fromUtf8 (_file.readAll()));
            configureDocument (file);

            _file.close();
        }

        else
        {
            QMessageBox::warning (this,
                                  tr ("Read error"),
                                  tr ("Cannot open file \"%1\"!\n%2")
                                  .arg (file)
                                  .arg (_file.errorString()));
        }
    }

    qApp->restoreOverrideCursor();
}

bool Editor::writeFile (const QString &file)
{
    if (!file.isEmpty())
    {
        QFile _file (file);

        if (_file.open (QIODevice::WriteOnly))
        {
            qApp->setOverrideCursor (Qt::WaitCursor);

            configureDocument (file);
            _file.write (text().toUtf8());
            _file.close();

            qApp->restoreOverrideCursor();
            return true;
        }

        else
        {
            QMessageBox _message;
            _message.setParent (this);
            _message.setIcon (QMessageBox::Warning);
            _message.setWindowTitle (tr ("Write error"));
            _message.setWindowModality (Qt::WindowModal);
            _message.setWindowIcon (QIcon (":/icons/dummy.png"));
            _message.setStandardButtons (QMessageBox::Yes | QMessageBox::No | QMessageBox::Discard);
            _message.setText ("<b>" + tr ("Cannot write data to file (%1)").arg (_file.errorString()) + "</b>");
            _message.setInformativeText (
                tr ("Do you want to save the document under a different name?"));

            int _return = _message.exec();

            if (_return == QMessageBox::Yes)
                return saveAs();

            else if (_return == QMessageBox::Discard)
                return true;
        }
    }

    return false;
}

void Editor::configureLexer (void)
{
    QsciLexer *_lexer;

    switch (m_current_language)
    {
        case Cpp:
            _lexer = new QsciLexerCPP (this);
            break;

        default:
            _lexer = new QsciLexerCPP (this);
    }

    setLexer (_lexer);

    lexer()->setFont (m_font);
    lexer()->setDefaultColor (m_theme->foreground());
    lexer()->setDefaultPaper (m_theme->background());
    lexer()->setAutoIndentStyle (QsciScintilla::AiMaintain);
}

void Editor::updateLineNumbers (void)
{
    if (m_line_numbers)
        setMarginWidth (1, QString ("00%1").arg (lines()));

    else
        setMarginWidth (1, 0);
}

void Editor::onMarginClicked (void)
{

}

void Editor::configureDocument (const QString &file)
{
    setModified (false);
    m_document_title = file;
    getProgrammingLanguage (file);

    emit updateTitle();
}

void Editor::getProgrammingLanguage (const QString &file)
{
    Q_UNUSED (file);

    m_current_language = Cpp;
    configureLexer();
}
