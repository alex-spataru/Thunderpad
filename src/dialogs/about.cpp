//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#include "about.h"

About::About(QWidget *parent) : QDialog(parent) {
    // Customize the dialog in order to look like
    // an utility dialog and not like a message.
    setWindowIcon(QIcon(":/icons/dummy.png"));
    setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint |
                   Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);

    // Hide the dialog & set the title
    setWindowTitle(" ");
    hide();

    // Create the widgets
    m_icon = new QLabel(this);
    m_name = new QLabel(this);
    m_version = new QLabel(this);
    m_copyright = new QLabel(this);
    m_layout = new QVBoxLayout(this);

    // Allign the labels at the horizontal center
    // of the dialog.
    m_name->setAlignment(Qt::AlignHCenter);
    m_icon->setAlignment(Qt::AlignHCenter);
    m_version->setAlignment(Qt::AlignHCenter);
    m_copyright->setAlignment(Qt::AlignHCenter);

    // Load the icon pixmap
    m_icon->setPixmap(QPixmap(":/icons/logo.png"));

    // Load the application name
    m_name->setText("<b>" + qApp->applicationName() + "</b>");

    // Configure the size of the font in the version and
    // copyright labels
    QString _font_size = "'2'";

    if (MAC_OS_X)
        _font_size = "'1'";

    // Load the application version
    m_version->setText("<font size=" + _font_size + ">" +
                       tr("Version %1").arg(qApp->applicationVersion()) +
                       "</font>");

    // Load the copyright text
    m_copyright->setText(
                "<font size=" + _font_size + ">" +
                tr("Copyright (C) 2013-2014 %1").arg(qApp->organizationName()) + "<br>" +
                tr("Released under the GNU GPL 3.0 License") + "</font>");

    // Add the widgets to an column-like layout
    m_layout->addWidget(m_icon);
    m_layout->addWidget(m_name);
    m_layout->addWidget(m_version);
    m_layout->addWidget(m_copyright);

    // Set the fixed size
    m_layout->setSizeConstraint(QLayout::SetFixedSize);
}
