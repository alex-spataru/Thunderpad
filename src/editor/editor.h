//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#ifndef EDITOR_H
#define EDITOR_H

#include <QIcon>
#include <QFile>
#include <QPainter>
#include <QSettings>
#include <QTextBlock>
#include <QFontDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QPlainTextEdit>

#include "theme.h"
#include "app_info.h"
#include "platform.h"
#include "line-numbers.h"
#include "spell-checker.h"
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

class Editor : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit Editor(QWidget *parent = 0);

    bool maybeSave();
    int lineNumberAreaWidth();
    void lineNumberAreaPaintEvent(QPaintEvent *event);

signals:
    void updateTitle();
    void settingsChanged();

public slots:
    void checkSpelling();
    void updateSettings();

    bool save();
    bool saveAs();

    void print();
    void exportPdf();
    void exportHtml();
    void selectFonts();
    void setReadOnly(bool ro);
    void setWordWrap(bool ww);
    void readFile(const QString &file);
    bool writeFile(const QString &file);
    void setSyntaxLanguage(const QString &language);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void highlightCurrentLine();
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &, int);

private:
    Theme *m_theme;
    QSettings *m_settings;
    QWidget *m_lineNumberArea;
    SyntaxHighlighter *m_highlighter;

    bool _hc_line_enabled;
};

#endif
