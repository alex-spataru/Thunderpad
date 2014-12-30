//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#include "about.h"

About::About(QWidget *parent) : QDialog(parent) {
    hide();
    setWindowFlags(Qt::Dialog |
                   Qt::CustomizeWindowHint |
                   Qt::WindowStaysOnTopHint |
                   Qt::WindowCloseButtonHint);

    QIcon _blank;
    setWindowIcon(_blank);
    setWindowTitle(tr("About %1").arg(APP_NAME));

    m_icon = new QLabel(this);
    m_name = new QLabel(this);
    m_information = new QLabel(this);
    m_layout = new QVBoxLayout(this);

    m_name->setAlignment(Qt::AlignHCenter);
    m_icon->setAlignment(Qt::AlignHCenter);
    m_information->setAlignment(Qt::AlignHCenter);

    m_layout->addWidget(m_icon);
    m_layout->addWidget(m_name);
    m_layout->addWidget(m_information);
    m_layout->setSizeConstraint(QLayout::SetFixedSize);

    m_icon->setPixmap(QPixmap(":/icons/logo.png"));
    m_name->setText("<b>" APP_NAME " " APP_VERSION "</b>");

    m_information->setText("<font size=2>"
                           "<p>" +
                           tr("Built on %1 at %2").arg(__DATE__, __TIME__) +
                           "</p><p>" +
                           tr("Copyright &copy; 2013-%1 %2")
                           .arg(QDateTime::currentDateTime().toString("yyyy"))
                           .arg(APP_COMPANY) + "<br>" +
                           tr("Released under the GNU GPL 3.0 License") +
                           "</p></font>");
}
