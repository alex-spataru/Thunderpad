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

#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QAction>
#include <QToolBar>
#include <QSettings>

#include "editor.h"
#include "window.h"

/*!
 * \class ToolBar
 *
 * The \c ToolBar class is in charge of creating and configuring
 * a new QToolbarar widget to be used with the \c Window class.
 *
 * This class creates the neccessary actions and connects the signals/slots
 * automatically with the initialize() function.
 */

class ToolBar : public QToolBar {
        Q_OBJECT

    public:
        ToolBar (Window *parent = 0);

    public slots:

        /*!
         * \brief setReadOnly
         * \param ro
         *
         * Shows or hides the read only button based on the \c ro parameter
         */

        void setReadOnly (bool ro);

        /*!
         * \brief setToolbarText
         * \param enabled
         *
         * Shows or hides the text under the toolbar icons
         * based on the \c enabled parameter
         */

        void setToolbarText (bool enabled);

        /*!
         * \brief setSaveEnabled
         * \param enabled
         *
         * Enables or disables the save button based on the \c enabled parameter
         */

        void setSaveEnabled (bool enabled);

    private slots:
        void updateSettings (void);
        void initialize (Window *window);
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
        QAction *m_find_replace;
        QAction *m_readonly;

        bool m_large_icons;
        bool m_toolbar_text;
        QSettings *m_settings;
};

#endif
