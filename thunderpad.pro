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
include($$PWD/libs/Fervor/Fervor.pri)
include($$PWD/libs/QtSingleApplication/QtSingleApplication.pri)

INCLUDEPATH += \
    $$PWD/src/app \
    $$PWD/src/dialogs \
    $$PWD/src/editor \
    $$PWD/src/editor/lexers \
    $$PWD/src/shared \
    $$PWD/src/window

# Add application icon and other shit in Windows
win32* {
    RC_FILE = $$PWD/data/windows/thunderpad.rc
}

# Add application icon and app configuration for OS X
macx* {
    ICON    = $$PWD/data/mac/icon.icns
    RC_FILE = $$PWD/data/mac/icon.icns
    QMAKE_INFO_PLIST = $$PWD/data/mac/info.plist
    QMAKE_POST_LINK = install_name_tool -change libqscintilla2.11.dylib $$[QT_INSTALL_LIBS]/libqscintilla2.11.dylib $(TARGET)
}

# Make the "make install" command useful in Linux and other
# operating systems
unix:!macx {
    target.path    = /usr/bin
    TARGET         = thunderpad
    desktop.path   = /usr/share/applications
    desktop.files += $$PWD/data/linux/thunderpad.desktop
    INSTALLS      += target desktop
    LIBS          += -lqscintilla2
}

# Import icons and other goodies to the application
RESOURCES += $$PWD/res/res.qrc

CONFIG += qscintilla2

HEADERS += \
    $$PWD/src/app/app.h \
    $$PWD/src/dialogs/searchdialog.h \
    $$PWD/src/editor/editor.h \
    $$PWD/src/window/menubar.h \
    $$PWD/src/window/toolbar.h \
    $$PWD/src/window/window.h \
    $$PWD/src/window/statusbar.h \
    $$PWD/src/shared/platform.h \
    $$PWD/src/editor/theme.h \
    $$PWD/src/shared/defaults.h \
    $$PWD/src/editor/lexer_database.h \
    $$PWD/src/editor/lexers/qscilexerada.h \
    $$PWD/src/editor/lexers/qscilexerasm.h \
    $$PWD/src/editor/lexers/qscilexerhaskell.h \
    $$PWD/src/editor/lexers/qscilexerlisp.h \
    $$PWD/src/editor/lexers/qscilexernsis.h \
    $$PWD/src/editor/lexers/qscilexerplaintext.h
    
SOURCES += \
    $$PWD/src/app/app.cpp \
    $$PWD/src/dialogs/searchdialog.cpp \
    $$PWD/src/editor/editor.cpp \
    $$PWD/src/window/menubar.cpp \
    $$PWD/src/window/toolbar.cpp \
    $$PWD/src/window/window.cpp \
    $$PWD/src/main.cpp \
    $$PWD/src/window/statusbar.cpp \
    $$PWD/src/editor/theme.cpp \
    $$PWD/src/editor/lexer_database.cpp \
    $$PWD/src/editor/lexers/qscilexerada.cpp \
    $$PWD/src/editor/lexers/qscilexerasm.cpp \
    $$PWD/src/editor/lexers/qscilexerhaskell.cpp \
    $$PWD/src/editor/lexers/qscilexerlisp.cpp \
    $$PWD/src/editor/lexers/qscilexernsis.cpp \
    $$PWD/src/editor/lexers/qscilexerplaintext.cpp
