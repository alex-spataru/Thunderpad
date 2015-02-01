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

#ifndef SETTINGS_H
#define SETTINGS_H

#include "platform.h"

//
// Statusbar defaults
//
#define SETTINGS_STATUSBAR_ENABLED true

//
// Editor defaults
//
#define DEFAULT_THEME "Light"
#define SETTINGS_CARET_LINE true
#define SETTINGS_CODE_FOLDING true
#define SETTINGS_LINE_NUMBERS true
#define SETTINGS_INDENTATION_WIDTH 4
#define SETTINGS_BRACE_MATCHING true
#define SETTINGS_WORD_WRAP_ENABLED true
#define SETTINGS_INDENTATION_GUIDES true

//
// Editor font
//
#define DEFAULT_FONT_SIZE 11
#define DEFAULT_FONT_FAMILY "Courier"

//
// Use Menlo font on OS X
//
#if MAC_OS_X
#undef DEFAULT_FONT_FAMILY
#define DEFAULT_FONT_FAMILY "Menlo"
#endif

//
// Use the Consolas font in Windows Vista or later
//
#if WINDOWS
#include <QSysInfo>
#undef DEFAULT_FONT_FAMILY
#define DEFAULT_FONT_FAMILY QSysInfo::windowsVersion() >= 0x0080 ? "Consolas" : "Courier New"
#endif

//
// Toolbar defaults
//
#define SETTINGS_TOOLBAR_TEXT false
#define SETTINGS_TOOLBAR_ENABLED true
#define SETTINGS_LARGE_ICONS MAC_OS_X
#define SETTINGS_ICON_THEME MAC_OS_X ? "Faience" : LINUX ? "Tango" : "Silk"

//
// Other defaults
//
#define SETTINGS_AUTO_CHECK_UPDATES false

#endif
