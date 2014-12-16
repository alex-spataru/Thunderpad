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

    // Attach the statusbar to the window
    window->setStatusBar(this);

    // Configure the statusbar label
    m_label = new QLabel(this);
    addPermanentWidget(m_label);

    // Get access to the window's text editor
    m_text_edit = window->editor();
    connect(m_text_edit, SIGNAL(textChanged()), this, SLOT(updateStatusLabel()));

    // Show/hide the statusbar when neccessary
    connect(window, SIGNAL(updateSettings()), this, SLOT(updateSettings()));
}

void StatusBar::updateSettings() {
    m_settings->value("statusbar-enabled", true).toBool() ? show() : hide();
}

void StatusBar::updateStatusLabel() {
    m_label->setText(fileSize() + "    " + wordCount() + "    " + lineCount());
}

QString StatusBar::fileSize() {
    QString _units;
    float _length = m_text_edit->toPlainText().length();

    if (_length < 1024)
        _units = " bytes";

    else if (_length < 1024 * 1024) {
        _length /= 1024;
        _units = " KB";
    }

    else {
        _length /= 1024 * 1024;
        _units = " MB";
    }

    return tr("Size:") + " " +
            QString::number(floorf(_length * 100 + 0.5) / 100) + _units;
}

QString StatusBar::wordCount() {
    long int _words =
            m_text_edit->toPlainText()
            .split(QRegExp("(\\s|\\n|\\r)+"), QString::SkipEmptyParts)
            .count();

    return tr("Words:") + " " + QString::number(_words);
}

QString StatusBar::lineCount() {
    return tr("Lines:") + " " +
            QString::number(m_text_edit->document()->blockCount());
}
