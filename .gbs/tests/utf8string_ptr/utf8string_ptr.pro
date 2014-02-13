include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_utf8string_ptr

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
#INCLUDEPATH += ../../../../cwt-test/include

SOURCES     += ../../../tests/utf8string_ptr.cpp

unix {
    LIBS += -lpfs
}

win32 {
    LIBS += pfs.lib
}
