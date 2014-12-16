//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#include "editor.h"

Editor::Editor(QWidget *parent) : QPlainTextEdit(parent) {
    setAttribute(Qt::WA_DeleteOnClose);

    m_theme = new Theme(this);
    m_lineNumberArea = new LineNumberArea(this);
    m_settings = new QSettings(APP_COMPANY, APP_NAME);
    m_highlighter = new SyntaxHighlighter(this->document());

    connect(this, SIGNAL(blockCountChanged(int)),
            this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect, int)),
            this, SLOT(updateLineNumberArea(QRect, int)));
    connect(this, SIGNAL(cursorPositionChanged()),
            this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
}

void Editor::lineNumberAreaPaintEvent(QPaintEvent *event) {
    QPainter _painter(m_lineNumberArea);
    _painter.fillRect(event->rect(), m_theme->lineNumbersBackground());
    _painter.setPen(m_theme->lineNumbersForeground());

    QTextBlock _block = firstVisibleBlock();
    int _blockNumber = _block.blockNumber();
    int _top =
            (int)blockBoundingGeometry(_block).translated(contentOffset()).top();
    int _bottom = _top + (int)blockBoundingRect(_block).height();

    while (_block.isValid() && _top <= event->rect().bottom()) {
        if (_block.isVisible() && _bottom >= event->rect().top()) {
            QString _number = QString::number(_blockNumber + 1);
            _painter.drawText(0, _top, m_lineNumberArea->width(),
                              fontMetrics().height(), Qt::AlignCenter, _number);
        }

        _block = _block.next();
        _top = _bottom;
        _bottom = _top + (int)blockBoundingRect(_block).height();
        ++_blockNumber;
    }
}

int Editor::lineNumberAreaWidth() {
    if (m_lineNumberArea->isEnabled()) {
        int _digits = 1;
        int _max = qMax(1, blockCount());

        while (_max >= 10) {
            _max /= 10;
            ++_digits;
        }

        return 16 + fontMetrics().width(QLatin1Char('9')) * _digits;
    }

    return 0;
}

void Editor::updateLineNumberAreaWidth(int) {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void Editor::updateLineNumberArea(const QRect &rect, int dy) {
    if (dy)
        m_lineNumberArea->scroll(0, dy);

    else
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(),
                                 rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void Editor::resizeEvent(QResizeEvent *e) {
    QPlainTextEdit::resizeEvent(e);
    m_lineNumberArea->setGeometry(
                QRect(contentsRect().left(), contentsRect().top(), lineNumberAreaWidth(),
                      contentsRect().height()));
}

void Editor::highlightCurrentLine() {
    QList<QTextEdit::ExtraSelection> _extra_selections;

    if (!isReadOnly() && _hc_line_enabled) {
        QTextEdit::ExtraSelection _selection;
        _selection.format.setBackground(m_theme->currentLineBackground());
        _selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        _selection.cursor = textCursor();
        _selection.cursor.clearSelection();
        _extra_selections.append(_selection);
    }

    setExtraSelections(_extra_selections);
}

void Editor::checkSpelling() {
    QString _dict_path = ":/dictionaries/en_US.dic";
    SpellChecker *spellChecker = new SpellChecker(_dict_path);
}

void Editor::updateSettings() {
    // Decide which font to use as a default based on the target
    // operating system
    int _default_size = 11;
    QString _default_font = "Courier";

    // Use Menlo (as in Xcode) in Mac OS X
    if (MAC_OS_X)
        _default_font = "Menlo";

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
    QFont _font;
    _font.setBold(m_settings->value("font-bold", false).toBool());
    _font.setItalic(m_settings->value("font-italic", false).toBool());
    _font.setUnderline(m_settings->value("font-underline", false).toBool());
    _font.setPointSize(m_settings->value("font-size", _default_size).toInt());
    _font.setFamily(m_settings->value("font-family", _default_font).toString());
    setFont(_font);

    // Set the word wrapping
    setWordWrap(m_settings->value("wordwrap-enabled", true).toBool());

    // Update the colors of the text editor
    m_theme->readTheme(m_settings->value("color-scheme", "Light").toString());
    QPalette _palette = palette();
    _palette.setColor(QPalette::Base, m_theme->background());
    _palette.setColor(QPalette::Text, m_theme->foreground());
    _palette.setColor(QPalette::Highlight, m_theme->highlightBackground());
    _palette.setColor(QPalette::HighlightedText, m_theme->highlightForeground());
    setPalette(_palette);

    // Update the current color of the line
    _hc_line_enabled = m_settings->value("hc-line-enabled", "true").toBool();
    highlightCurrentLine();

    // Show/hide the line numbers
    m_lineNumberArea->setEnabled(
                m_settings->value("line-numbers-enabled", "true").toBool());
    m_lineNumberArea->setVisible(m_lineNumberArea->isEnabled());
    updateLineNumberAreaWidth(blockCount());

    // Update the highlighter colors
    m_highlighter->updateColor(m_theme);
}

bool Editor::save() {
    return documentTitle().isEmpty() ? saveAs() : writeFile(documentTitle());
}

bool Editor::saveAs() {
    return writeFile(QFileDialog::getSaveFileName(this, tr("Save as") + "...",
                                                  QDir::homePath()));
}

bool Editor::maybeSave() {
    // We don't need to save the document if it isn't modified
    if (!document()->isModified())
        return true;

    // The document was already saved in the hard disk, however,
    // it has unsaved changes.
    if (!documentTitle().isEmpty() && document()->isModified())
        return save();

    // The document was never saved as a file, so we need to prompt
    // the user where to save our document.
    else if (document()->isModified()) {
        QMessageBox _message;
        _message.setParent(this);
        _message.setWindowTitle(" ");
        _message.setWindowModality(Qt::WindowModal);
        _message.setWindowIcon(QIcon(":/icons/dummy.png"));
        _message.setIconPixmap(QPixmap(":/icons/logo.png"));
        _message.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel |
                                    QMessageBox::Discard);
        _message.setText(
                    "<b>" + tr("This document has changes, do you want to save them?") +
                    "</b>");

        _message.setInformativeText(
                    tr("Your changes will be lost if you close this item without saving."));

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

void Editor::print() {
    // TODO
}

void Editor::exportPdf() {
    // TODO
}

void Editor::exportHtml() {
    // TODO
}

void Editor::selectFonts() {
    // Create a new QFontDialog and select the same font
    // as the text editor
    QFontDialog _dialog;
    _dialog.setCurrentFont(font());

    // Only save and apply new font if the user clicked on
    // the "Accept" || "Ok" button
    if (_dialog.exec() == QFontDialog::Accepted) {
        setFont(_dialog.selectedFont());

        // As explained before, we need to define every
        // important detail about the font when saving it
        // into QSettings.
        m_settings->setValue("font-bold", font().bold());
        m_settings->setValue("font-italic", font().italic());
        m_settings->setValue("font-family", font().family());
        m_settings->setValue("font-size", font().pointSize());
        m_settings->setValue("font-underline", font().underline());

        // Notify the rest of the program that the settings must
        // be synced.
        emit settingsChanged();
    }
}

void Editor::setReadOnly(bool ro) {
    setEnabled(!ro);
}

void Editor::setWordWrap(bool ww) {
    setWordWrapMode(ww ? QTextOption::WrapAtWordBoundaryOrAnywhere
                       : QTextOption::NoWrap);
}

void Editor::readFile(const QString &file) {
    // Only read valid paths
    if (!file.isEmpty()) {

        // Create a file and open it in Read Only mode
        QFile _file(file);

        if (_file.open(QIODevice::ReadOnly)) {

            // Read the contents of the file and display them
            // in the text editor
            setPlainText(QString::fromUtf8(_file.readAll()));

            // Update the document state
            document()->setModified(false);
            setDocumentTitle(file);
            emit updateTitle();
        }

        // Close the file so that it can be used by the system
        _file.close();
    }
}

bool Editor::writeFile(const QString &file) {
    // Only use valid paths
    if (!file.isEmpty()) {

        // Create a file and open it i Write Only mode
        QFile _file(file);

        if (_file.open(QIODevice::WriteOnly)) {

            // Write the contents of the text editor
            // into the file
            _file.write(toPlainText().toUtf8());

            // Update the document state
            document()->setModified(false);
            setDocumentTitle(file);
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

void Editor::setSyntaxLanguage(const QString &language) {
    m_highlighter->setLanguage(language);
}
