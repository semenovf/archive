include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_utf8string_ptr

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include

SOURCES     += ../../../tests/utf8string_ptr.cpp

unix {
    LIBS += -lpfs
}

win32 {
    LIBS += pfs.lib
}
