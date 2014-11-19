//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "window.h"

#include <qaction.h>
#include <qtoolbar.h>
#include <qsettings.h>

/*!
 * \class ToolBar
 *
 * The \c ToolBar class is in charge of creating and configuring
 * a new QToolbarar widget to be used with the \c Window class.
 *
 * This class creates the neccessary actions and connects the signals/slots
 * automatically with the connectSlots() function.
 */


class ToolBar : public QToolBar
{
        Q_OBJECT

    public:
        ToolBar (QWidget *parent = 0);
        void connectSlots (Window *window);

        public
    slots:
        void updateSettings();
        void setReadOnly (bool ro);
        void setToolbarText (bool enabled);
        void setSaveEnabled (bool enabled);
        void update_theme (const QString &theme);

    private:
        QAction *m_new;
        QAction *m_open;
        QAction *m_save;
        QAction *m_print;
        QAction *m_undo;
        QAction *m_redo;
        QAction *m_cut;
        QAction *m_copy;
        QAction *m_paste;
        QAction *m_replace;
        QAction *m_readonly;

        bool m_old_value;
        QSettings *m_settings;
};

#endif
