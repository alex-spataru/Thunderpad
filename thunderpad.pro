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

include($$PWD/app/3rd-party/QSimpleUpdater/QSimpleUpdater.pri)

INCLUDEPATH += \
    $$PWD/app/src/app \
    $$PWD/app/src/dialogs \
    $$PWD/app/src/editor \
    $$PWD/app/src/shared \
    $$PWD/app/src/window

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
    $$PWD/app/res/res.qrc

HEADERS += \
    $$PWD/app/src/app/app.h \
    $$PWD/app/src/dialogs/about.h \
    $$PWD/app/src/dialogs/find.h \
    $$PWD/app/src/editor/editor.h \
    $$PWD/app/src/editor/line-numbers.h \
    $$PWD/app/src/editor/spell-checker.h \
    $$PWD/app/src/editor/syntax-highlighter.h \
    $$PWD/app/src/window/menubar.h \
    $$PWD/app/src/window/toolbar.h \
    $$PWD/app/src/window/window.h \
    $$PWD/app/src/window/statusbar.h \
    $$PWD/app/src/shared/platform.h \
    $$PWD/app/src/editor/theme.h \
    $$PWD/app/src/shared/app_info.h \
    app/src/dialogs/find.h \
    app/src/dialogs/findreplace.h

SOURCES += \
    $$PWD/app/src/app/app.cpp \
    $$PWD/app/src/dialogs/about.cpp \
    $$PWD/app/src/editor/editor.cpp \
    $$PWD/app/src/editor/line-numbers.cpp \
    $$PWD/app/src/editor/spell-checker.cpp \
    $$PWD/app/src/editor/syntax-highlighter.cpp \
    $$PWD/app/src/window/menubar.cpp \
    $$PWD/app/src/window/toolbar.cpp \
    $$PWD/app/src/window/window.cpp \
    $$PWD/app/src/main.cpp \
    $$PWD/app/src/window/statusbar.cpp \
    $$PWD/app/src/editor/theme.cpp \
    app/src/dialogs/find.cpp \
    app/src/dialogs/findreplace.cpp

FORMS += \
    app/src/dialogs/find.ui \
    app/src/dialogs/findreplace.ui
