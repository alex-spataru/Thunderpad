#
#  This file is part of Thunderpad
#
#  Copyright (c) 2013-2014 Alex Spataru <alex.racotta@gmail.com>
#  Please check the license.txt file for more information.
#

TEMPLATE   = app
TARGET     = thunderpad
VERSION    = 1.0

CODECFORTR  = UTF-8
CODECFORSRC = UTF-8

QT += gui
QT += xml
QT += network
QT += widgets
QT += printsupport

include($$PWD/libs/Hunspell/Hunspell.pri)
include($$PWD/libs/QSimpleUpdater/QSimpleUpdater.pri)

INCLUDEPATH += \
    $$PWD/src/app \
    $$PWD/src/dialogs \
    $$PWD/src/editor \
    $$PWD/src/shared \
    $$PWD/src/window

win32* {
    TARGET  = "Thunderpad"
    RC_FILE = $$PWD/data/windows/icon.rc
}

macx* {
    TARGET  = "Thunderpad"
    ICON    = $$PWD/data/mac/icon.icns
    RC_FILE = $$PWD/data/mac/icon.icns
    QMAKE_INFO_PLIST = $$PWD/data/mac/info.plist
}

unix:!macx {
    target.path    = /usr/bin
    desktop.path   = /usr/share/applications
    desktop.files += $$PWD/data/linux/thunderpad.desktop
    INSTALLS      += target desktop
}

RESOURCES += $$PWD/res/res.qrc

HEADERS += \
    $$PWD/src/app/app.h \
    $$PWD/src/dialogs/searchdialog.h \
    $$PWD/src/editor/editor.h \
    $$PWD/src/editor/line-numbers.h \
    $$PWD/src/editor/spell-checker.h \
    $$PWD/src/editor/syntax-highlighter.h \
    $$PWD/src/window/menubar.h \
    $$PWD/src/window/toolbar.h \
    $$PWD/src/window/window.h \
    $$PWD/src/window/statusbar.h \
    $$PWD/src/shared/platform.h \
    $$PWD/src/editor/theme.h \
    $$PWD/src/shared/assembly_info.h \

SOURCES += \
    $$PWD/src/app/app.cpp \
    $$PWD/src/dialogs/searchdialog.cpp \
    $$PWD/src/editor/editor.cpp \
    $$PWD/src/editor/line-numbers.cpp \
    $$PWD/src/editor/spell-checker.cpp \
    $$PWD/src/editor/syntax-highlighter.cpp \
    $$PWD/src/window/menubar.cpp \
    $$PWD/src/window/toolbar.cpp \
    $$PWD/src/window/window.cpp \
    $$PWD/src/main.cpp \
    $$PWD/src/window/statusbar.cpp \
    $$PWD/src/editor/theme.cpp
