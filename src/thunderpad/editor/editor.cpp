//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#include "editor.h"

/*!
 * \brief Editor::Editor
 * \param parent
 *
 * Initializes and configures the editor
 */

Editor::Editor (QWidget *parent) : QPlainTextEdit (parent)
{
    setAttribute (Qt::WA_DeleteOnClose);

    m_theme = new Theme (this);
    m_lineNumberArea = new LineNumberArea (this);
    m_highlighter = new SyntaxHighlighter (this->document());

    connect (this, SIGNAL (blockCountChanged (int)), this,
             SLOT (updateLineNumberAreaWidth (int)));
    connect (this, SIGNAL (updateRequest (QRect, int)), this,
             SLOT (updateLineNumberArea (QRect, int)));
    connect (this, SIGNAL (cursorPositionChanged()), this,
             SLOT (highlightCurrentLine()));
    updateLineNumberAreaWidth (0);
}

/*!
 * \brief Editor::lineNumberAreaPaintEvent
 * \param event
 *
 * Paints and configures a widget that shows the current line number
 */

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

/*!
 * \brief Editor::lineNumberAreaWidth
 * \return
 *
 * Calculates the width to be used for the line number area
 */

int Editor::lineNumberAreaWidth()
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

/*!
 * \brief Editor::updateLineNumberAreaWidth
 *
 * Applies the calculated width for the line number area
 */

void Editor::updateLineNumberAreaWidth (int)
{
    setViewportMargins (lineNumberAreaWidth(), 0, 0, 0);
}

/*!
 * \brief Editor::updateLineNumberArea
 * \param rect
 * \param dy
 *
 * Updates the size and display of the line number area
 */

void Editor::updateLineNumberArea (const QRect &rect, int dy)
{
    if (dy)
        m_lineNumberArea->scroll (0, dy);

    else
        m_lineNumberArea->update (0, rect.y(), m_lineNumberArea->width(),
                                  rect.height());

    if (rect.contains (viewport()->rect())) updateLineNumberAreaWidth (0);
}

/*!
 * \brief Editor::resizeEvent
 * \param e represents the resize event
 *
 * Updates the location of the line number area
 */

void Editor::resizeEvent (QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent (e);
    m_lineNumberArea->setGeometry (
        QRect (contentsRect().left(), contentsRect().top(), lineNumberAreaWidth(),
               contentsRect().height()));
}

/*!
 * \brief Editor::highlightCurrentLine
 *
 * Changes the background color of the current line to "highlight" it.
 */

void Editor::highlightCurrentLine()
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

/*!
 * \brief Editor::updateSettings
 *
 * Reads and applies the new settings in the text editor
 */

void Editor::updateSettings()
{
    QSettings _settings ("Alex Spataru", "Thunderpad");

    // Decide which font to use as a default based on the target
    // operating system
    int _default_size = 11;
    QString _default_font = "Courier";

    // Use Menlo (as in Xcode) in Mac OS X
    if (MAC_OS_X) _default_font = "Menlo";

    // Use Consolas in Windows Vista and later.
    // If detected OS is older, then use Courier New. We need to use
    // the preprocessors because we use the windowsVersion() function.
#if WINDOWS
    _default_size = 10;

    if (QSysInfo::windowsVersion() >= 0x0080)
        _default_font = "Consolas";

    else
        _default_font = "Courier New";

#endif

    // We need to take care of every aspect of the font, such as the family,
    // the size and the style in an individual manner because QSettings
    // does not support saving fonts directly
    QFont _settings_font;
    _settings_font.setFamily (
        _settings.value ("font-family", _default_font).toString());
    _settings_font.setPointSize (
        _settings.value ("font-size", _default_size).toInt());
    _settings_font.setBold (_settings.value ("font-bold", false).toBool());
    _settings_font.setItalic (_settings.value ("font-italic", false).toBool());
    _settings_font.setUnderline (
        _settings.value ("font-underline", false).toBool());
    setFont (_settings_font);

    // Set the word wrapping
    setWordWrap (_settings.value ("wordwrap-enabled", true).toBool());

    // Update the colors of the text editor
    m_theme->readTheme (_settings.value ("color-scheme", "Light").toString());
    QPalette _palette = palette();
    _palette.setColor (QPalette::Base, m_theme->background());
    _palette.setColor (QPalette::Text, m_theme->foreground());
    _palette.setColor (QPalette::Highlight, m_theme->highlightBackground());
    _palette.setColor (QPalette::HighlightedText, m_theme->highlightForeground());
    setPalette (_palette);

    // Update the current color of the line
    _hc_line_enabled = _settings.value ("hc-line-enabled", "true").toBool();
    highlightCurrentLine();

    // Show/hide the line numbers
    m_lineNumberArea->setEnabled (
        _settings.value ("line-numbers-enabled", "true").toBool());
    m_lineNumberArea->setVisible (m_lineNumberArea->isEnabled());
    updateLineNumberAreaWidth (blockCount());

    // Update the highlighter colors
    m_highlighter->updateColor (m_theme);
}

/*!
 * \brief Editor::save
 * \return
 *
 * Decides whenever to save the file directly or to open a "Save As" dialog
 */

bool Editor::save()
{
    return documentTitle().isEmpty() ? saveAs() : writeFile (documentTitle());
}

/*!
 * \brief Editor::saveAs
 * \return
 *
 * Opens a "Save As" dialog and writes the current file.
 */

bool Editor::saveAs()
{
    return writeFile (QFileDialog::getSaveFileName (this, tr ("Save as") + "...",
                      QDir::homePath()));
}

/*!
 * \brief Editor::maybeSave
 * \return
 *
 * Decides if we should save the current file automatically or
 * to ask the user to save the current file.
 */

bool Editor::maybeSave()
{
    // We don't need to save the document if it isn't modified
    if (!document()->isModified()) return true;

    // The document was already saved in the hard disk, however,
    // it has unsaved changes.
    if (!documentTitle().isEmpty() && document()->isModified()) return save();

    // The document was never saved as a file, so we need to prompt
    // the user where to save our document.
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

/*!
 * \brief Editor::print
 *
 * Shows a dialog that will guide the user to print
 * the current document
 */

void Editor::print() {}

/*!
 * \brief Editor::exportPdf
 *
 * Exports the current file as a PDF file.
 */

void Editor::exportPdf() {}

/*!
 * \brief Editor::exportHtml
 *
 * Exports the current file as a HTML file.
 */

void Editor::exportHtml() {}

/*!
 * \brief Editor::selectFonts
 *
 * Shows a QFontDialog and allows the user to change the font used in the
 * text editor.
 */

void Editor::selectFonts()
{
    // Create a new QFontDialog and select the same font
    // as the text editor
    QFontDialog _dialog;
    _dialog.setCurrentFont (font());

    // Only save and apply new font if the user clicked on
    // the "Accept" || "Ok" button
    if (_dialog.exec() == QFontDialog::Accepted)
    {
        setFont (_dialog.selectedFont());

        // As explained before, we need to define every
        // important detail about the font when saving it
        // into QSettings.
        QSettings _settings ("Alex Spataru", "Thunderpad");
        _settings.setValue ("font-family", font().family());
        _settings.setValue ("font-size", font().pointSize());
        _settings.setValue ("font-bold", font().bold());
        _settings.setValue ("font-italic", font().italic());
        _settings.setValue ("font-underline", font().underline());

        // Notify the rest of the program that the settings must
        // be synced.
        emit settingsChanged();
    }
}

/*!
 * \brief Editor::setReadOnly
 * \param ro
 *
 * Toggles the read only feature of the text editor.
 */

void Editor::setReadOnly (bool ro)
{
    // Lock/unlock the text editor
    setEnabled (!ro);
}

/*!
 * \brief Editor::setWordWrap
 * \param ww
 *
 * Toggles the state of the word wrapping feature
 */

void Editor::setWordWrap (bool ww)
{
    // Enable word wrapping
    if (ww) setWordWrapMode (QTextOption::WrapAtWordBoundaryOrAnywhere);

    // Disable word wrapping
    else
        setWordWrapMode (QTextOption::NoWrap);
}

/*!
 * \brief Editor::find
 * \param query
 * \return
 *
 * Searches, highlights results for a string and returns
 * the number of matches for the query.
 */

int Editor::find (const QString &query)
{
    Q_UNUSED (query);
    return 0;
}

/*!
 * \brief Editor::replace
 * \param query
 *
 * Uses the Editor::find() function to search for a query
 * and replaces the first match of the query.
 */

void Editor::replace (const QString &query)
{
    Q_UNUSED (query);
}

/*!
 * \brief Editor::replaceAll
 * \param query
 *
 * Uses the Editor::find() and Editor::replace() function
 * to replace all matches for the query
 */

void Editor::replaceAll (const QString &query)
{
    Q_UNUSED (query);
}

/*!
 * \brief Editor::readFile
 * \param file the file to open
 *
 * Opens and loads a file into the text editor
 */

void Editor::readFile (const QString &file)
{
    // Only read valid paths
    if (!file.isEmpty())
    {

        // Create a file and open it in Read Only mode
        QFile _file (file);

        if (_file.open (QIODevice::ReadOnly))
        {

            // Read the contents of the file and display them
            // in the text editor
            setPlainText (QString::fromUtf8 (_file.readAll()));

            // Update the document state
            document()->setModified (false);
            setDocumentTitle (file);
            emit updateTitle();
        }

        // Close the file so that it can be used by the system
        _file.close();
    }
}

/*!
 * \brief Editor::writeFile
 * \param file the file to write
 * \return
 *
 * Saves the contents of the text editor in \param file.
 * Returns \c true if the saving is successful.
 */

bool Editor::writeFile (const QString &file)
{
    // Only use valid paths
    if (!file.isEmpty())
    {

        // Create a file and open it i Write Only mode
        QFile _file (file);

        if (_file.open (QIODevice::WriteOnly))
        {

            // Write the contents of the text editor
            // into the file
            _file.write (toPlainText().toUtf8());

            // Update the document state
            document()->setModified (false);
            setDocumentTitle (file);
            emit updateTitle();
        }

        // Close the file
        _file.close();

        // Tell the rest of the program that the saving was
        // successfull.
        return true;
    }

    // Tell the rest of the program that the saving was not
    // successfull.
    return false;
}

/*!
 * \brief Editor::setSyntaxLanguage
 * \param language
 *
 * Calls the Hightlighter::setLanguage() function and
 * and inputs it the \c language paramter.
 */

void Editor::setSyntaxLanguage (const QString &language)
{
    m_highlighter->setLanguage (language);
}
