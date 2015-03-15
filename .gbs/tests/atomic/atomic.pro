include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_cwt_atomic

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
#INCLUDEPATH += ../../../../cwt-test/include

SOURCES += ../../../tests/atomic.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
}

win32 {
    LIBS += pfs.lib
    LIBS += pfs-sys.lib
}
