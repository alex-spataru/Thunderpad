//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#include <math.h>
#include <QLabel>
#include <QString>
#include <QRegExp>
#include <QSettings>
#include <QStatusBar>
#include <QPlainTextEdit>

#include "window.h"
#include "app_info.h"

/*!
 * \class StatusBar
 *
 * The \c StatusBar class is in charge of creating and configuring
 * a new QStatusBar widget to be used with the \c Window class.
 */

class StatusBar : public QStatusBar {
    Q_OBJECT

public:
    StatusBar(Window *parent = 0);

private slots:
    void updateSettings(void);
    void updateStatusLabel(void);
    void initialize(Window *window);

private:
    QLabel *m_label;
    QSettings *m_settings;
    QPlainTextEdit *m_text_edit;

    QString fileSize(void);
    QString wordCount(void);
    QString lineCount(void);
};
