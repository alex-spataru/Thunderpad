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

#include "qscilexerplaintext.h"

QsciLexerPlainText::QsciLexerPlainText (Editor *parent) : QsciLexer (parent)
{
}

const char *QsciLexerPlainText::keywords (int set) const
{
    Q_UNUSED (set);
    return 0;
}

QString QsciLexerPlainText::description (int style) const
{
    Q_UNUSED (style);
    return "";
}

QColor QsciLexerPlainText::defaultColor (int style) const
{
    Q_UNUSED (style);
    return Qt::black;
}
