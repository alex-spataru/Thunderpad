//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2015 Alex Spataru <alexspataru@outlook.com>
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

#include "lexer_database.h"
#include "lexers/plain_text.h"

#include <QFile>
#include <QFileInfo>

#include <Qsci/qscilexerbash.h>
#include <Qsci/qscilexerbatch.h>
#include <Qsci/qscilexercmake.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexercsharp.h>
#include <Qsci/qscilexercss.h>
#include <Qsci/qscilexerd.h>
#include <Qsci/qscilexerdiff.h>
#include <Qsci/qscilexerfortran.h>
#include <Qsci/qscilexerfortran77.h>
#include <Qsci/qscilexerhtml.h>
#include <Qsci/qscilexeridl.h>
#include <Qsci/qscilexerjava.h>
#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qscilexerlua.h>
#include <Qsci/qscilexermakefile.h>
#include <Qsci/qscilexermatlab.h>
#include <Qsci/qscilexerpascal.h>
#include <Qsci/qscilexerperl.h>
#include <Qsci/qscilexerpostscript.h>
#include <Qsci/qscilexerpov.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexerruby.h>
#include <Qsci/qscilexerspice.h>
#include <Qsci/qscilexersql.h>
#include <Qsci/qscilexertcl.h>
#include <Qsci/qscilexertex.h>
#include <Qsci/qscilexerverilog.h>
#include <Qsci/qscilexervhdl.h>
#include <Qsci/qscilexerxml.h>
#include <Qsci/qscilexeryaml.h>

/*!
 * \class LexerDatabase
 * \brief Configures appropiate lexers for the \c Editor
 *
 * The \c LexerDatabase is in charge of creating, configuring
 * and loading the correct \c QsciLexer for the \c Editor automatically
 */

/*! \internal
 * Initializes the class and configures the theme
 */

LexerDatabase::LexerDatabase (void) {}

/*!
 * Creates a new lexer based on the \c file name and
 * configures it to fit the needs of the \a Editor.
 */

QsciLexer *LexerDatabase::getLexer (const QString &file)
{
    QsciLexer *_lexer = _guessByName (file);
    return _lexer;
}

/*!
 * \internal
 * Returns a appropiate QsciLexer given the input \a {file}
 */

QsciLexer *LexerDatabase::_guessByName (const QString &file)
{
    QsciLexer *lexer;
    QFile _file (file);
    QString s = QFileInfo (_file).suffix().toLower();
    QString n = QFileInfo (_file).baseName().toLower();

    //
    // Bash files
    //
    if (s == "sh" || s == "bsh")
        lexer = new QsciLexerBash();

    //
    // Batch files
    //
    else if (s == "cmd" || s == "bat" ||
             s == "btm" || s == "nt")
        lexer = new QsciLexerBatch();

    //
    // CMake files
    //
    else if (s == "cmake" || n == "cmakelists")
        lexer = new QsciLexerCMake();

    //
    // C and C++ files
    //
    else if (s == "cpp" || s == "cxx" ||
             s == "cc"  || s == "hh"  ||
             s == "h"   || s == "hpp")
        lexer = new QsciLexerCPP();

    //
    // C# files
    //
    else if (s == "cs")
        lexer = new QsciLexerCSharp();

    //
    // CSS files
    //
    else if (s == "css")
        lexer = new QsciLexerCSS();

    //
    // D files
    //
    else if (s == "d")
        lexer = new QsciLexerD();

    //
    // Diff
    //
    else if (s == "diff" || s == "patch")
        lexer = new QsciLexerDiff();

    //
    // Fortran files
    //
    else if (s == "f90" || s == "f95" ||
             s == "f03" || s == "f15" ||
             s == "f2k")
        lexer = new QsciLexerFortran();

    //
    // Fortran77 files
    //
    else if (s == "f" || s == "for")
        lexer = new QsciLexerFortran77();

    //
    // HTML
    //
    else if (s == "html" || s == "htm")
        lexer = new QsciLexerHTML();

    //
    // IDL
    //

    //
    // Java
    //
    else if (s == "java")
        lexer = new QsciLexerJava();

    //
    // Javascript
    //
    else if (s == "js")
        lexer = new QsciLexerJavaScript();

    //
    // Lua
    //
    else if (s == "lua")
        lexer = new QsciLexerLua();

    //
    // Makefile
    //
    else if (s == "mak" || n == "gnumakefile" ||
             n == "makefile")
        lexer = new QsciLexerMakefile();

    //
    // Matlab
    //
    else if (s == "m")
        lexer = new QsciLexerMatlab();

    //
    // Pascal
    //
    else if (s == "pas" || s == "inc")
        lexer = new QsciLexerPascal();

    //
    // Perl
    //
    else if (s == "pl" || s == "pm" ||
             s == "plx")
        lexer = new QsciLexerPerl();

    //
    // Postscript
    //
    else if (s == "ps")
        lexer = new QsciLexerPostScript();

    //
    // POV
    //

    //
    // Python
    //
    else if (s == "py" || s == "pyw")
        lexer = new QsciLexerPython();

    //
    // Ruby
    //
    else if (s == "rb" || s == "rbw")
        lexer = new QsciLexerRuby();

    //
    // Spice
    //

    //
    // SQL
    //
    else if (s == "sql")
        lexer = new QsciLexerSQL();

    //
    // TCL
    //
    else if (s == "tcl")
        lexer = new QsciLexerTCL();

    //
    // TeX
    //
    else if (s == "tex")
        lexer = new QsciLexerTeX();

    //
    // Verilog
    //
    else if (s == "v"  || s == "sv" ||
             s == "vh" || s == "svh")
        lexer = new QsciLexerVerilog();

    //
    // VHDL
    //
    else if (s == "vhd" || s == " vhdl")
        lexer = new QsciLexerVHDL();

    //
    // XML
    //
    else if (s == "xml"  || s == "xsl"  ||
             s == "xsml" || s == "xsd"  ||
             s == "kml"  || s == "wsdl" ||
             s == "xlf"  || s == "xliff")
        lexer = new QsciLexerXML();

    //
    // YAML
    //
    else if (s == "yml")
        lexer = new QsciLexerYAML();

    //
    // Other shit
    //
    else
        lexer = new QsciLexerPlainText();

    return lexer;
}
