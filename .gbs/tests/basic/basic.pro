include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_io

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/basic.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
    LIBS += -lpfs-io
}

win32 {
}
