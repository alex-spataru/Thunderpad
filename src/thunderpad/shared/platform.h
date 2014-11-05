//
//  This file is part of Thunderpad
//
//  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
//  Please check the license.txt file for more information.
//

#ifndef PLATFORM_H
#define PLATFORM_H

#ifndef LINUX
#define LINUX false
#endif

#ifndef WINDOWS
#define WINDOWS false
#endif

#ifndef MAC_OS_X
#define MAC_OS_X false
#endif

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
