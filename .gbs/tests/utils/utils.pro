include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_utils

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/utils.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
}

win32 {
    LIBS += pfs.lib
    LIBS += pfs-sys.lib
}
