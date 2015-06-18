include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_io_utf8

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/utf8.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
    LIBS += -lpfs-io
}

win32 {
}
