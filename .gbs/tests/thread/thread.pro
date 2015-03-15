include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_thread

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../../cwt-sys/include

SOURCES += ../../../tests/thread.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
}

win32 {
    LIBS += pfs.lib
    LIBS += pfs-sys.lib
}
