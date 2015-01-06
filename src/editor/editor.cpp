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

#include "editor.h"

Editor::Editor (QWidget *parent) : QPlainTextEdit (parent)
{
    setAttribute (Qt::WA_DeleteOnClose);

    m_theme = new Theme (this);
    m_lineNumberArea = new LineNumberArea (this);
    m_settings = new QSettings (APP_COMPANY, APP_NAME);
    m_highlighter = new SyntaxHighlighter (this->document());

    connect (this, SIGNAL (blockCountChanged (int)),
             this, SLOT (updateLineNumberAreaWidth (int)));
    connect (this, SIGNAL (updateRequest (QRect, int)),
             this, SLOT (updateLineNumberArea (QRect, int)));
    connect (this, SIGNAL (cursorPositionChanged()),
             this, SLOT (highlightCurrentLine()));

    updateLineNumberAreaWidth (0);
}

void Editor::lineNumberAreaPaintEvent (QPaintEvent *event)
{
    QPainter _painter (m_lineNumberArea);
    _painter.fillRect (event->rect(), m_theme->lineNumbersBackground());
    _painter.setPen (m_theme->lineNumbersForeground());

    QTextBlock _block = firstVisibleBlock();
    int _blockNumber = _block.blockNumber();
    int _top =
        (int)blockBoundingGeometry (_block).translated (contentOffset()).top();
    int _bottom = _top + (int)blockBoundingRect (_block).height();

    while (_block.isValid() && _top <= event->rect().bottom())
        {
        if (_block.isVisible() && _bottom >= event->rect().top())
            {
            QString _number = QString::number (_blockNumber + 1);
            _painter.drawText (0, _top, m_lineNumberArea->width(),
                               fontMetrics().height(), Qt::AlignCenter, _number);
            }

        _block = _block.next();
        _top = _bottom;
        _bottom = _top + (int)blockBoundingRect (_block).height();
        ++_blockNumber;
        }
}

int Editor::lineNumberAreaWidth (void)
{
    if (m_lineNumberArea->isEnabled())
        {
        int _digits = 1;
        int _max = qMax (1, blockCount());

        while (_max >= 10)
            {
            _max /= 10;
            ++_digits;
            }

        return 16 + fontMetrics().width (QLatin1Char ('9')) * _digits;
        }

    return 0;
}

void Editor::updateLineNumberAreaWidth (int)
{
    setViewportMargins (lineNumberAreaWidth(), 0, 0, 0);
}

void Editor::updateLineNumberArea (const QRect &rect, int dy)
{
    if (dy)
        m_lineNumberArea->scroll (0, dy);

    else
        m_lineNumberArea->update (0,
                                  rect.y(),
                                  m_lineNumberArea->width(),
                                  rect.height());

    if (rect.contains (viewport()->rect()))
        updateLineNumberAreaWidth (0);
}

void Editor::resizeEvent (QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent (e);
    m_lineNumberArea->setGeometry (QRect (contentsRect().left(),
                                          contentsRect().top(),
                                          lineNumberAreaWidth(),
                                          contentsRect().height()));
}

void Editor::highlightCurrentLine (void)
{
    QList<QTextEdit::ExtraSelection> _extra_selections;

    if (!isReadOnly() && _hc_line_enabled)
        {
        QTextEdit::ExtraSelection _selection;
        _selection.format.setBackground (m_theme->currentLineBackground());
        _selection.format.setProperty (QTextFormat::FullWidthSelection, true);
        _selection.cursor = textCursor();
        _selection.cursor.clearSelection();
        _extra_selections.append (_selection);
        }

    setExtraSelections (_extra_selections);
}

void Editor::configureDocument (const QString &file)
{
    Q_UNUSED (file);
    Q_ASSERT (!file.isEmpty());

    setDocumentTitle (file);
    document()->setModified (false);
    m_highlighter->detectLanguage (file);

    emit updateTitle();
}

void Editor::checkSpelling (void)
{
    // TODO
}

void Editor::updateSettings (void)
{
    int _default_size = 11;
    QString _default_font = "Courier";

#if MAC_OS_X
    _default_font = "Menlo";
#endif

#if WINDOWS
    _default_size = 10;
    _default_font = QSysInfo::windowsVersion() >= 0x0080 ? "Consolas" : "Courier New";
#endif

#if LINUX
    // Get system monospaced font?
#endif

    // Load saved font
    QFont _font;
    _font.setBold (m_settings->value ("font-bold", false).toBool());
    _font.setItalic (m_settings->value ("font-italic", false).toBool());
    _font.setUnderline (m_settings->value ("font-underline", false).toBool());
    _font.setPointSize (m_settings->value ("font-size", _default_size).toInt());
    _font.setFamily (m_settings->value ("font-family", _default_font).toString());
    setFont (_font);

    // Set the word wrapping
    setWordWrap (m_settings->value ("wordwrap-enabled", true).toBool());

    // Update the colors of the text editor
    m_theme->readTheme (m_settings->value ("color-scheme", "Light").toString());
    m_highlighter->updateColor (m_theme);

    QPalette _palette = palette();
    _palette.setColor (QPalette::Base, m_theme->background());
    _palette.setColor (QPalette::Text, m_theme->foreground());
    _palette.setColor (QPalette::Highlight, m_theme->highlightBackground());
    _palette.setColor (QPalette::HighlightedText, m_theme->highlightForeground());
    setPalette (_palette);

    // Update the current color of the highlighted line
    _hc_line_enabled = m_settings->value ("hc-line-enabled", "true").toBool();
    highlightCurrentLine();

    // Show/hide the line numbers
    m_lineNumberArea->setEnabled (
        m_settings->value ("line-numbers-enabled", "true").toBool());
    m_lineNumberArea->setVisible (m_lineNumberArea->isEnabled());
    updateLineNumberAreaWidth (blockCount());
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

bool Editor::maybeSave (void)
{
    // We don't need to save the document if it isn't modified
    if (!document()->isModified())
        return true;

    // The document was already saved in the hard disk, however, it has unsaved changes.
    if (!documentTitle().isEmpty() && document()->isModified())
        return save();

    // The document was never saved in the hard disk, so we prompt the user
    else if (document()->isModified())
        {
        QMessageBox _message;
        _message.setParent (this);
        _message.setWindowTitle (" ");
        _message.setWindowModality (Qt::WindowModal);
        _message.setWindowIcon (QIcon (":/icons/dummy.png"));
        _message.setIconPixmap (QPixmap (":/icons/logo.png"));
        _message.setStandardButtons (QMessageBox::Save | QMessageBox::Cancel |
                                     QMessageBox::Discard);
        _message.setText (
            "<b>" + tr ("This document has changes, do you want to save them?") +
            "</b>");

        _message.setInformativeText (
            tr ("Your changes will be lost if you close this item without saving."));

        int _return_ = _message.exec();

        if (_return_ == QMessageBox::Save)
            return save();

        else if (_return_ == QMessageBox::Discard)
            return true;

        else
            return false;
        }

    return false;
}

void Editor::goToLine (void)
{
    // TODO
}

void Editor::sortSelection (void)
{
    // TODO
}

void Editor::insertDateTime (void)
{
    // TODO
}

void Editor::print (void)
{
    qApp->setOverrideCursor (Qt::WaitCursor);

    QPrinter _printer (QPrinter::HighResolution);
    QPrintDialog *_dialog = new QPrintDialog (&_printer, this);

    _printer.setDocName (documentTitle());
    _printer.setFontEmbeddingEnabled (true);

    if (_dialog->exec() == QDialog::Accepted)
        document()->print (&_printer);

    qApp->restoreOverrideCursor();
}

void Editor::exportPdf (void)
{
    // Ask user where to save PDF
    QString _file_location =
        QFileDialog::getSaveFileName (this,
                                      tr ("Export PDF"),
                                      QDir::homePath(),
                                      "*.pdf");

    qApp->setOverrideCursor (Qt::WaitCursor);

    // Write PDF file
    if (!_file_location.isEmpty())
        {
        QPrinter _printer (QPrinter::HighResolution);

        _printer.setDocName (documentTitle());
        _printer.setFontEmbeddingEnabled (true);
        _printer.setOutputFileName (_file_location);
        _printer.setOutputFormat (QPrinter::PdfFormat);

        document()->print (&_printer);
        }

    qApp->restoreOverrideCursor();
}

void Editor::exportHtml (void)
{
    QString _file_location =
        QFileDialog::getSaveFileName (this,
                                      tr ("Export HTML"),
                                      QDir::homePath(),
                                      "*.html");

    qApp->setOverrideCursor (Qt::WaitCursor);

    if (!_file_location.isEmpty())
        {
        QFile _file (_file_location);

        if (_file.open (QIODevice::WriteOnly | QIODevice::Text))
            {
            _file.write (document()->toHtml().toUtf8());
            _file.close();
            }

        else
            {
            QMessageBox::warning (this,
                                  tr ("Write error"),
                                  tr ("Cannot write data to %1.\n%2")
                                  .arg (_file_location)
                                  .arg (_file.errorString()));
            }

        qApp->restoreOverrideCursor();
        }
}

void Editor::selectFonts (void)
{
    QFontDialog _dialog;
    _dialog.setCurrentFont (font());

    if (_dialog.exec() == QFontDialog::Accepted)
        {
        setFont (_dialog.selectedFont());

        m_settings->setValue ("font-bold", font().bold());
        m_settings->setValue ("font-italic", font().italic());
        m_settings->setValue ("font-family", font().family());
        m_settings->setValue ("font-size", font().pointSize());
        m_settings->setValue ("font-underline", font().underline());

        emit settingsChanged();
        }
}

void Editor::setWordWrap (bool ww)
{
    setWordWrapMode (ww ? QTextOption::WrapAtWordBoundaryOrAnywhere
                     : QTextOption::NoWrap);
}

void Editor::readFile (const QString &file)
{
    qApp->setOverrideCursor (Qt::WaitCursor);

    if (!file.isEmpty())
        {
        QFile _file (file);

        // File can be opened
        if (_file.open (QIODevice::ReadOnly))
            {
            setPlainText (QString::fromUtf8 (_file.readAll()));
            configureDocument (file);

            _file.close();
            }

        // File is read protected
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

        // We can write to the file
        if (_file.open (QIODevice::WriteOnly))
            {
            qApp->setOverrideCursor (Qt::WaitCursor);

            configureDocument (file);
            _file.write (toPlainText().toUtf8());
            _file.close();

            qApp->restoreOverrideCursor();
            return true;
            }

        // The file is write-protected
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

void Editor::setSyntaxLanguage (const QString &language)
{
    m_highlighter->setLanguage (language);
}
