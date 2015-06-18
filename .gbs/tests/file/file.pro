include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_io_file

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/file.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
    LIBS += -lpfs-io
}

win32 {
}
