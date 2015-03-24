include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_unitype

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include

SOURCES     += ../../../tests/unitype.cpp

unix {
    LIBS += -lpfs
}

win32 {
    LIBS += pfs.lib
}
