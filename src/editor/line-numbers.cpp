//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#include "editor.h"
#include "line-numbers.h"

/*!
 * \brief LineNumberArea::LineNumberArea
 * \param editor
 *
 * Initializes the LineNumberArea widget and defines in which widget
 * the LineNumberArea will be placed on.
 */

LineNumberArea::LineNumberArea (Editor *editor) : QWidget (editor) {
    m_editor = editor;
}

/*!
 * \brief LineNumberArea::sizeHint
 * \return
 *
 * Returns the width of the line number area widget by calling the
 * lineNumberAreaWidth() function of our parent.
 */

QSize LineNumberArea::sizeHint() const {
    return QSize (m_editor->lineNumberAreaWidth(), 0);
}

/*!
 * \brief LineNumberArea::paintEvent
 * \param event
 *
 * Calls the lineNumberAreaPaintEvent() function of our
 * parent widget.
 * The LineNumberAreaPaintEvent() function allows us to display the
 * line numbers label for each painted line in the text editor.
 */

void LineNumberArea::paintEvent (QPaintEvent *event) {
    m_editor->lineNumberAreaPaintEvent (event);
}
