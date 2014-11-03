//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#include <qstatusbar.h>

/*!
 * \class StatusBar
 *
 * The \c StatusBar class is in charge of creating and configuring
 * a new QStatusBar widget to be used with the \c Window class.
 *
 * This class creates the neccessary actions and connects the signals/slots
 * automatically with the connectSlots() function.
 */


class StatusBar : public QStatusBar
{
        Q_OBJECT

    public:
        StatusBar (QWidget *parent = 0);
};
