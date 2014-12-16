//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#ifndef LINE_NUMBERS_H
#define LINE_NUMBERS_H

#include <QSize>

class Editor;

/*!
 * \class LineNumberArea
 *
 * The LineNumberArea class implements a simple \c QWidget that the
 * \c Editor uses to display line numbers.
 */

class LineNumberArea : public QWidget {

public:
    LineNumberArea(Editor *editor);
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);

private:
    Editor *m_editor;
};

#endif
