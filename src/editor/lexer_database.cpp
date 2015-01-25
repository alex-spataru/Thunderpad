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

#include "lexer_database.h"
#include "lexers/plain_text.h"

/*!
 * \class LexerDatabase
 * \brief Configures appropiate lexers for the \c Editor
 *
 * The \c LexerDatabase is in charge of creating, configuring
 * and loading the correct \c QsciLexer for the \c Editor automatically
 */

/*! \internal
 * Initializes the class by doing nothing.
 */

LexerDatabase::LexerDatabase (void) {}

/*!
 * Returns an appropiate QsciLexer given the \a {file_name}.
 */

QsciLexer *LexerDatabase::getLexer (const QString &file_name)
{
    Q_UNUSED (file_name);

    QsciLexer *_lexer;
    _lexer = new QsciLexerPlainText();

    return _lexer;
}
