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

#ifndef QSCILEXER_PLAIN_TEXT_H
#define QSCILEXER_PLAIN_TEXT_H

#ifdef __APPLE__
extern "C++" {
#endif

#include "editor.h"
#include <Qsci/qscilexer.h>

class QsciLexerPlainText : public QsciLexer {

        Q_OBJECT

    public:
        explicit QsciLexerPlainText (Editor *parent = 0);

        const char *keywords (int set) const;
        QString description (int style) const;
        QColor defaultColor (int style) const;
        const char *lexer() const {
            return "plain_text";
        }
        const char *language() const {
            return "PlainText";
        }
};

#endif

#ifdef __APPLE__
}
#endif
