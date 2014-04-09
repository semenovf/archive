include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_fsm

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include

SOURCES += ../../../tests/fsm.cpp

unix {
    LIBS += -lpfs
}

win32 {
    LIBS += pfs.lib
}
