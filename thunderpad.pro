#
#  This file is part of Thunderpad
#
#  Copyright (c) 2013-2015 Alex Spataru <alex.racotta@gmail.com>
#  Please check the license.txt file for more information.
#

TEMPLATE = app

# General application information
VERSION = 0.9.3
TARGET  = Thunderpad

# Assembly info used in C++
DEFINES +=  APP_NAME=\\\"$$TARGET\\\"
DEFINES +=  APP_VERSION=\\\"$$VERSION\\\"
DEFINES += "APP_COMPANY=\"\\\"Alex Spataru\\\"\""

# Use UTF-8 in the source code and translation files
# This prevents getting nasty text renderings when using
# "non-standard" characters.
CODECFORTR  = UTF-8
CODECFORSRC = UTF-8

# Extra Qt modules
QT += gui
QT += xml
QT += network
QT += widgets
QT += printsupport

# 3rd-party libraries
include(libs/Fervor/Fervor.pri)
include(libs/QtSingleApplication/QtSingleApplication.pri)

INCLUDEPATH += \
    src/app \
    src/dialogs \
    src/editor \
    src/editor/lexers \
    src/shared \
    src/window

# Add application icon and other shit in Windows
win32* {
    RC_FILE = data/windows/thunderpad.rc
}

# Add application icon and app configuration for OS X
macx* {
    ICON    = data/mac/icon.icns
    RC_FILE = data/mac/icon.icns
    QMAKE_INFO_PLIST = data/mac/info.plist
    QMAKE_POST_LINK = install_name_tool -change libqscintilla2.11.dylib $$[QT_INSTALL_LIBS]/libqscintilla2.11.dylib $(TARGET)
}

# Make the "make install" command useful in Linux and other
# operating systems
unix:!macx {
    target.path    = /usr/bin
    TARGET         = thunderpad
    desktop.path   = /usr/share/applications
    desktop.files += data/linux/thunderpad.desktop
    INSTALLS      += target desktop
    LIBS          += -lqscintilla2
}

# Import icons and other goodies to the application
RESOURCES += res/res.qrc

CONFIG += qscintilla2

HEADERS += \
    src/app/app.h \
    src/dialogs/searchdialog.h \
    src/editor/editor.h \
    src/window/menubar.h \
    src/window/toolbar.h \
    src/window/window.h \
    src/window/statusbar.h \
    src/shared/platform.h \
    src/editor/theme.h \
    src/shared/defaults.h \
    src/editor/lexer_database.h \
    src/editor/lexers/qscilexerada.h \
    src/editor/lexers/qscilexerasm.h \
    src/editor/lexers/qscilexerhaskell.h \
    src/editor/lexers/qscilexerlisp.h \
    src/editor/lexers/qscilexernsis.h \
    src/editor/lexers/qscilexerplaintext.h
    
SOURCES += \
    src/app/app.cpp \
    src/dialogs/searchdialog.cpp \
    src/editor/editor.cpp \
    src/window/menubar.cpp \
    src/window/toolbar.cpp \
    src/window/window.cpp \
    src/main.cpp \
    src/window/statusbar.cpp \
    src/editor/theme.cpp \
    src/editor/lexer_database.cpp \
    src/editor/lexers/qscilexerada.cpp \
    src/editor/lexers/qscilexerasm.cpp \
    src/editor/lexers/qscilexerhaskell.cpp \
    src/editor/lexers/qscilexerlisp.cpp \
    src/editor/lexers/qscilexernsis.cpp \
    src/editor/lexers/qscilexerplaintext.cpp
