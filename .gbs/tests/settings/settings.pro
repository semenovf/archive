include($$(GBS_HOME)/qmake/common-app-qt.pri)

TARGET = test_pfs_settings
DEFINES += __CWT_TEST__

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs-sys/include
INCLUDEPATH += ../../../../pfs-app/include

SOURCES     += ../../../tests/settings.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
    LIBS += -lpfs-app
}

win32 {
#    LIBS += pfs.lib
}
