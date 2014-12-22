//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#include "editor.h"
#include "line-numbers.h"

LineNumberArea::LineNumberArea(Editor *editor) : QWidget(editor) {
    m_editor = editor;
}

QSize LineNumberArea::sizeHint(void) const {
    return QSize(m_editor->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event) {
    m_editor->lineNumberAreaPaintEvent(event);
}
