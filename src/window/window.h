//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#ifndef WINDOW_H
#define WINDOW_H

#include <QFile>
#include <QSettings>
#include <QCloseEvent>
#include <QMainWindow>
#include <QApplication>
#include <QSimpleUpdater>
#include <QDesktopServices>

#include "about.h"
#include "editor.h"
#include "app_info.h"

class ToolBar;
class MenuBar;
class StatusBar;
class SearchDialog;

/*!
 * \class Window
 *
 * The \c Window class is used to display and configure each element
 * of Thunderpad, such as the \c Editor widget.
 *
 * The \c Window is in charge of connecting and configuring everything together,
 * such as the actions of the menubar with the functions of the text editor.
 * Finally, the \c Window allows the \c Application class to create a new window
 * and open a file using the configureWindow() function.
 */

class MenuBar;
class ToolBar;

class Window : public QMainWindow {
    Q_OBJECT

public:
    Window();

    Editor *editor() const;
    ToolBar *toolbar() const;

    void configureWindow(Window *window);
    void openFile(const QString &file_name);

signals:
    void updateSettings();
    void settingsChanged();
    void checkForUpdates();
    void readOnlyChanged(bool ro);

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void newFile();
    void open();

    void setReadOnly(bool ro);
    void setWordWrap(bool ww);
    void setToolbarText(bool tt);
    void setToolbarEnabled(bool tb);
    void setStatusBarEnabled(bool sb);
    void setHCLineEnabled(bool hc);
    void setLineNumbersEnabled(bool ln);
    void setIconTheme(const QString &theme);
    void setColorscheme(const QString &colorscheme);

    void showFindReplaceDialog();

    void aboutThunderpad();
    void license();
    void donate();
    void viewHelp();
    void reportBug();
    void sendFeedback();
    void makeContribution();
    void officialWebsite();

private slots:
    void updateTitle();
    void syncSettings();
    void saveWindowState();

private:
    MenuBar *m_menu;
    Editor *m_editor;
    About *m_about_dlg;
    ToolBar *m_toolbar;
    QSettings *m_settings;
    StatusBar *m_statusbar;
    SearchDialog *m_search_dialog;

    QSimpleUpdater *m_updater;
    QString shortFileName(const QString &file);
};

#endif
