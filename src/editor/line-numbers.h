//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2015 Alex Spataru <alex_spataru@outlook.com>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301
//  USA
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
        LineNumberArea (Editor *editor);
        QSize sizeHint (void) const;

    protected:
        void paintEvent (QPaintEvent *event);

    private:
        Editor *m_editor;
};

#endif
