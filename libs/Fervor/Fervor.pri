QT += core gui webkit webkitwidgets network

isEmpty(FV_APP_NAME) {
	DEFINES += FV_APP_NAME=\\\"$$TARGET\\\"
} else {
	DEFINES += FV_APP_NAME=\\\"$$FV_APP_NAME\\\"
}

isEmpty(FV_APP_VERSION) {
	DEFINES += FV_APP_VERSION=\\\"$$VERSION\\\"
} else {
	DEFINES += FV_APP_VERSION=\\\"$$FV_APP_VERSION\\\"
}

DEPENDPATH += "$$PWD/src"
INCLUDEPATH += "$$PWD/src"

SOURCES += \
        $$PWD/src/fvupdatewindow.cpp \
        $$PWD/src/fvupdater.cpp \
        $$PWD/src/fvversioncomparator.cpp \
        $$PWD/src/fvplatform.cpp \
        $$PWD/src/fvignoredversions.cpp \
        $$PWD/src/fvavailableupdate.cpp \
        $$PWD/src/fvdownloaddialog.cpp

HEADERS += \
        $$PWD/src/fvupdatewindow.h \
        $$PWD/src/fvupdater.h \
        $$PWD/src/fvversioncomparator.h \
        $$PWD/src/fvplatform.h \
        $$PWD/src/fvignoredversions.h \
        $$PWD/src/fvavailableupdate.h \
        $$PWD/src/fvdownloaddialog.h

FORMS += $$PWD/ui/fvupdatewindow.ui \
         $$PWD/ui/fvdownloaddialog.ui

TRANSLATIONS += $$PWD/i18n/fervor_lt.ts
CODECFORTR = UTF-8

RESOURCES += $$PWD/res/fv_resources.qrc
