#
#  This file is part of Thunderpad
#
#  Copyright (c) 2013-2015 Alex Spataru <alex.racotta@gmail.com>
#  Please check the license.txt file for more information.
#

TEMPLATE   = app
TARGET     = thunderpad
VERSION    = 0.9.2

CODECFORTR  = UTF-8
CODECFORSRC = UTF-8

CONFIG += qscintilla2

QT += gui
QT += xml
QT += network
QT += widgets
QT += printsupport

include($$PWD/libs/QSimpleUpdater/QSimpleUpdater.pri)
include($$PWD/libs/QtSingleApplication/QtSingleApplication.pri)

INCLUDEPATH += \
    $$PWD/src/app \
    $$PWD/src/dialogs \
    $$PWD/src/editor \
    $$PWD/src/shared \
    $$PWD/src/window

win32* {
    TARGET  = "Thunderpad"
    RC_FILE = $$PWD/data/windows/thunderpad.rc
}

macx* {
    TARGET  = "Thunderpad"
    ICON    = $$PWD/data/mac/icon.icns
    RC_FILE = $$PWD/data/mac/icon.icns
    QMAKE_INFO_PLIST = $$PWD/data/mac/info.plist
    QMAKE_POST_LINK = install_name_tool -change libqscintilla2.11.dylib $$[QT_INSTALL_LIBS]/libqscintilla2.11.dylib $(TARGET)
}

unix:!macx {
    target.path    = /usr/bin
    desktop.path   = /usr/share/applications
    desktop.files += $$PWD/data/linux/thunderpad.desktop
    INSTALLS      += target desktop
    LIBS          += -lqscintilla2
}

RESOURCES += $$PWD/res/res.qrc

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
    $$PWD/src/shared/assembly_info.h \
    $$PWD/src/shared/defaults.h \
    $$PWD/src/editor/lexers/plain_text.h \
    src/editor/lexer_database.h

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
    $$PWD/src/editor/lexers/plain_text.cpp \
    src/editor/lexer_database.cpp
