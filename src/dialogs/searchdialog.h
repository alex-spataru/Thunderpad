//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#ifndef SearchDialog_H
#define SearchDialog_H

#include <QColor>
#include <QLabel>
#include <QDialog>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QTextCursor>
#include <QGridLayout>
#include <QTextDocument>
#include <QPlainTextEdit>

#include "window.h"

class SearchDialog : public QDialog {
    Q_OBJECT

public:
    SearchDialog(Window *parent = 0);

private slots:
    void search();
    void replaceAll();
    void replaceFirstOccurrence();

private:
    QTextCursor *m_text_cursor;
    QPlainTextEdit *m_text_edit;

    QLabel *ui_find_label;
    QGridLayout *ui_layout;
    QLabel *ui_replace_label;
    QLineEdit *ui_find_lineedit;
    QPushButton *ui_done_button;
    QPushButton *ui_replace_button;
    QLineEdit *ui_replace_lineedit;
    QSpacerItem *ui_vertical_spacer;
    QCheckBox *ui_match_case_checkbox;
    QCheckBox *ui_whole_words_checkbox;
    QPushButton *ui_replace_all_button;
};

#endif
