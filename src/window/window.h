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

#ifndef WINDOW_H
#define WINDOW_H

class Editor;
class ToolBar;
class MenuBar;
class ToolBar;
class MenuBar;
class StatusBar;
class QSettings;
class QMainWindow;
class SearchDialog;

#include <QMainWindow>

class Window : public QMainWindow {
        Q_OBJECT

    public:
        Window (const QString &file);
        ~Window (void);

        Editor *editor (void) const;
        ToolBar *toolbar (void) const;
        MenuBar *menubar (void) const;
        QSettings *settings (void) const;
        SearchDialog *searchDialog (void) const;

        void configureWindow (Window *window);
        void openFile (const QString &file_name);

    signals:
        void updateSettings (void);
        void settingsChanged (void);
        void checkForUpdates (void);
        void readOnlyChanged (bool ro);

    protected:
        void moveEvent (QMoveEvent *event);
        void closeEvent (QCloseEvent *event);
        void resizeEvent (QResizeEvent *event);

    public slots:
        void newFile (void);
        void open (void);
        void setReadOnly (bool ro);
        void setWordWrap (bool ww);
        void setToolbarText (bool tt);
        void setToolbarEnabled (bool tb);
        void setStatusBarEnabled (bool sb);
        void setHCLineEnabled (bool hc);
        void setUseLargeIcons (bool li);
        void setLineNumbersEnabled (bool ln);
        void setIconTheme (const QString &theme);
        void setColorscheme (const QString &colorscheme);
        void showFindReplaceDialog (void);
        void aboutThunderpad (void);
        void license (void);
        void donate (void);
        void viewHelp (void);
        void reportBug (void);
        void sendFeedback (void);
        void makeContribution (void);
        void officialWebsite (void);

    private slots:
        void updateTitle (void);
        void syncSettings (void);
        void saveWindowState (void);
        QString shortFileName (const QString &file);

    private:
        MenuBar *m_menu;
        Editor *m_editor;
        ToolBar *m_toolbar;
        QSettings *m_settings;
        StatusBar *m_statusbar;
        SearchDialog *m_search_dialog;
};

#endif
