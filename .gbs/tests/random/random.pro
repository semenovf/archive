include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_random

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include

SOURCES += ../../../tests/random.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
}

win32 {
    LIBS += pfs.lib
    LIBS += pfs-sys.lib
}
