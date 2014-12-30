//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#include "statusbar.h"

StatusBar::StatusBar(Window *parent) : QStatusBar(parent) {
    m_settings = new QSettings(APP_COMPANY, APP_NAME);
    updateSettings();

    initialize(parent);
}

void StatusBar::initialize(Window *window) {
    Q_ASSERT(window != NULL);

    window->setStatusBar(this);
    m_text_edit = window->editor();

    m_size_label = new QLabel(this);
    m_lines_label = new QLabel(this);
    m_words_label = new QLabel(this);

    addPermanentWidget(m_size_label);
    addPermanentWidget(m_lines_label);
    addPermanentWidget(m_words_label);

    connect(m_text_edit, SIGNAL(textChanged()), this, SLOT(updateStatusLabel()));
    connect(window, SIGNAL(updateSettings()), this, SLOT(updateSettings()));
}

void StatusBar::updateSettings(void) {
    m_settings->value("statusbar-enabled", true).toBool() ? show() : hide();
}

void StatusBar::updateStatusLabel(void) {
    m_size_label->setText("  " + fileSize() + "  ");
    m_lines_label->setText("  " + lineCount() + "  ");
    m_words_label->setText("  " + wordCount() + "  ");
}

QString StatusBar::fileSize(void) {
    QString _units;
    float _length = m_text_edit->toPlainText().length();

    // File is less than one KB
    if (_length < 1024)
        _units = " bytes";

    // File is one KB or greater, but smaller than one MB
    else if (_length < 1024 * 1024) {
        _length /= 1024;
        _units = " KB";
    }

    // File is one MB or greater
    else {
        _length /= 1024 * 1024;
        _units = " MB";
    }

    return tr("Size:") + " " +
            QString::number(floorf(_length * 100 + 0.5) / 100) + _units;
}

QString StatusBar::wordCount(void) {
    long int _words =
            m_text_edit->toPlainText()
            .split(QRegExp("(\\s|\\n|\\r)+"), QString::SkipEmptyParts)
            .count();

    return tr("Words:") + " " + QString::number(_words);
}

QString StatusBar::lineCount(void) {
    return tr("Lines:") + " " +
            QString::number(m_text_edit->document()->blockCount());
}
