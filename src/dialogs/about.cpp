//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex_spataru@outlook.com>
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

#include "about.h"

About::About(QWidget *parent) : QDialog(parent) {
    hide();

    QIcon _blank;
    setWindowIcon(_blank);

    setWindowModality(Qt::WindowModal);
    setWindowFlags(Qt::Dialog |
                   Qt::CustomizeWindowHint |
                   Qt::WindowCloseButtonHint);

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
