include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_io_addrinfo

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/addrinfo.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
    LIBS += -lpfs-io
}

win32 {
}
