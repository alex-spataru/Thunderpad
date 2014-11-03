//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#include <qicon.h>
#include <qlabel.h>
#include <qdialog.h>
#include <qlayout.h>
#include <qapplication.h>

#include "platform.h"

/*!
 * \class About
 *
 * The \c About class implements a QDialog that shows information about the
 * current version of Thunderpad.
 */

class About : public QDialog
{
        Q_OBJECT

    public:
        explicit About (QWidget *parent = 0);

    private:
        QLabel *m_icon;
        QLabel *m_name;
        QLabel *m_version;
        QLabel *m_copyright;

        QVBoxLayout *m_layout;
};
