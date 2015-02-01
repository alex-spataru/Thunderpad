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

#ifndef PLATFORM_H
#define PLATFORM_H

#define LINUX false
#define WINDOWS false
#define MAC_OS_X false

#ifdef Q_OS_LINUX
#undef LINUX
#define LINUX true
#endif

#ifdef Q_OS_WIN32
#undef WINDOWS
#define WINDOWS true
#endif

#ifdef Q_OS_MAC
#undef MAC_OS_X
#define MAC_OS_X true
#endif

#endif
