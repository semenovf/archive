include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_utf8string

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include

SOURCES     += ../../../tests/utf8string.cpp

unix {
    LIBS += -lpfs
}

win32 {
    LIBS += pfs.lib
}
