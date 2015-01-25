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

#ifndef LEXERDATABASE_H
#define LEXERDATABASE_H

#include <QObject>
#include <Qsci/qscilexer.h>

enum Lexers
{
    Avs,
    Bash,
    Batch,
    CMake,
    CoffeeScript,
    CPP,
    CSharp,
    CSS,
    D,
    Diff,
    Fortran,
    Fortran77,
    Html,
    Idl,
    Java,
    Javascript,
    Lua,
    Makefile,
    Matlab,
    Octave,
    Pascal,
    Perl,
    Po,
    Pov,
    Python,
    Ruby,
    Spice,
    Sql,
    Tcl,
    Tex,
    Verilog,
    Vhdl,
    Xml,
    Yaml,
    PlainText
};

class LexerDatabase : public QObject
{
        Q_OBJECT

    public:
        explicit LexerDatabase (void);

    public slots:
        QsciLexer *getLexer (const QString &file_name);

    private:
        Lexers m_current_lexer;
};

#endif
