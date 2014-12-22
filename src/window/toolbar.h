//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QAction>
#include <QToolBar>
#include <QSettings>

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
    ToolBar(Window *parent = 0);

public slots:

    /*!
     * \brief setReadOnly
     * \param ro
     *
     * Shows or hides the read only button based on the \c ro parameter
     */

    void setReadOnly(bool ro);

    /*!
     * \brief setToolbarText
     * \param enabled
     *
     * Shows or hides the text under the toolbar icons
     * based on the \c enabled parameter
     */

    void setToolbarText(bool enabled);

    /*!
     * \brief setSaveEnabled
     * \param enabled
     *
     * Enables or disables the save button based on the \c enabled parameter
     */

    void setSaveEnabled(bool enabled);

private slots:
    void updateSettings(void);
    void initialize(Window *window);
    void update_theme(const QString &theme);

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
