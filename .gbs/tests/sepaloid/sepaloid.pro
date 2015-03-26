include($$(GBS_HOME)/qmake/common-test.pri)
TARGET = test_pfs_sepaloid

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../../pfs-sys/include
INCLUDEPATH += ../../../../pfs-app/include

SOURCES += ../../../tests/sepaloid.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
    LIBS += -lpfs-app
}

win32 {
}
