HEADERS += \
    $$PWD/qtlocalpeer.h \
    $$PWD/qtlockedfile.h \
    $$PWD/qtsingleapplication.h \
    $$PWD/qtsinglecoreapplication.h

SOURCES += \
    $$PWD/qtlocalpeer.cpp \
    $$PWD/qtlockedfile_unix.cpp \
    $$PWD/qtlockedfile_win.cpp \
    $$PWD/qtlockedfile.cpp \
    $$PWD/qtsingleapplication.cpp \
    $$PWD/qtsinglecoreapplication.cpp

INCLUDEPATH += $$PWD
