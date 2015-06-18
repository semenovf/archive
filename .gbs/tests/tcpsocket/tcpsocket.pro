include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_io_tcpsocket

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/tcpsocket.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
    LIBS += -lpfs-io
}

win32 {
}
