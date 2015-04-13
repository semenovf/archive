include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_io_tcpsocket

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../../pfs-sys/include
INCLUDEPATH += ../../../../pfs-io/include

SOURCES += ../../../tests/tcpsocket.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
    LIBS += -lpfs-io
}

win32 {
}
