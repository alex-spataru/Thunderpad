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

include($$PWD/src/3rd-party/Hunspell/Hunspell.pri)
include($$PWD/src/3rd-party/QSimpleUpdater/QSimpleUpdater.pri)

INCLUDEPATH += \
    $$PWD/src/thunderpad/app \
    $$PWD/src/thunderpad/dialogs \
    $$PWD/src/thunderpad/editor \
    $$PWD/src/thunderpad/shared \
    $$PWD/src/thunderpad/window

win32* {
    TARGET  = "Thunderpad"
    RC_FILE = $$PWD/data/windows/icon.rc
}

macx* {
    LIBS += -lcrypto -lssl
    TARGET  = "Thunderpad"
    ICON    = $$PWD/data/mac/icon.icns
    RC_FILE = $$PWD/data/mac/icon.icns
    CONFIG += app_bundle
    QMAKE_INFO_PLIST = $$PWD/data/mac/info.plist
}

unix:!macx {
    LIBS += -lcrypto -lssl
    target.path    = /usr/bin
    desktop.path   = /usr/share/applications
    desktop.files += $$PWD/data/linux/thunderpad.desktop
    INSTALLS      += target desktop
}

RESOURCES += \
    $$PWD/src/resources/resources.qrc

HEADERS += \
    $$PWD/src/thunderpad/app/app.h \
    $$PWD/src/thunderpad/dialogs/about.h \
    $$PWD/src/thunderpad/dialogs/find.h \
    $$PWD/src/thunderpad/editor/editor.h \
    $$PWD/src/thunderpad/editor/line-numbers.h \
    $$PWD/src/thunderpad/editor/spell-checker.h \
    $$PWD/src/thunderpad/editor/syntax-highlighter.h \
    $$PWD/src/thunderpad/window/menubar.h \
    $$PWD/src/thunderpad/window/toolbar.h \
    $$PWD/src/thunderpad/window/window.h \
    $$PWD/src/thunderpad/window/statusbar.h \
    $$PWD/src/thunderpad/shared/platform.h \
    $$PWD/src/thunderpad/editor/theme.h \
    $$PWD/src/thunderpad/shared/app_info.h \
    $$PWD/src/thunderpad/dialogs/find.h \
    $$PWD/src/thunderpad/dialogs/findreplace.h

SOURCES += \
    $$PWD/src/thunderpad/app/app.cpp \
    $$PWD/src/thunderpad/dialogs/about.cpp \
    $$PWD/src/thunderpad/editor/editor.cpp \
    $$PWD/src/thunderpad/editor/line-numbers.cpp \
    $$PWD/src/thunderpad/editor/spell-checker.cpp \
    $$PWD/src/thunderpad/editor/syntax-highlighter.cpp \
    $$PWD/src/thunderpad/window/menubar.cpp \
    $$PWD/src/thunderpad/window/toolbar.cpp \
    $$PWD/src/thunderpad/window/window.cpp \
    $$PWD/src/thunderpad/main.cpp \
    $$PWD/src/thunderpad/window/statusbar.cpp \
    $$PWD/src/thunderpad/editor/theme.cpp \
    $$PWD/src/thunderpad/dialogs/find.cpp \
    $$PWD/src/thunderpad/dialogs/findreplace.cpp

FORMS += \
    $$PWD/src/thunderpad/dialogs/find.ui \
    $$PWD/src/thunderpad/dialogs/findreplace.ui
