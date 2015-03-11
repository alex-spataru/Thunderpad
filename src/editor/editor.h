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

#ifndef EDITOR_H
#define EDITOR_H

#ifdef __APPLE__
extern "C++" {
#endif

class Theme;
class QSettings;
class LexerDatabase;

#include <Qsci/qsciscintilla.h>

class Editor : public QsciScintilla {
        Q_OBJECT

    public:
        explicit Editor (QWidget *parent = 0);

        bool maybeSave (void);
        int wordCount (void);
        bool titleIsShit (void);
        QString calculateSize (void);
        QString documentTitle (void) const;

    signals:
        void updateTitle (void);
        void settingsChanged (void);

    public slots:
        void exportPdf (void);
        void resetZoom (void);
        void documentInfo (void);
        void updateSettings (void);
        bool save (void);
        bool saveAs (void);
        void goToLine (void);
        void sortSelection (void);
        void insertDateTime (void);
        void print (void);
        void selectFonts (void);
        void setWordWrap (bool ww);
        void readFile (const QString &file);
        bool writeFile (const QString &file);

    private slots:
        void updateLexer (void);
        void updateLineNumbers (void);
        void onMarginClicked (void);
        void configureDocument (const QString &file);

    private:
        Theme *theme (void) const;
        QSettings *settings (void) const;
	LexerDatabase *lexerDatabase (void) const;

        QFont m_font;
        bool m_line_numbers;
        QString m_document_title;
};

#endif

#ifdef __APPLE__
}
#endif
