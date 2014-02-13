include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_utf8string

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
#INCLUDEPATH += ../../../../cwt-test/include

SOURCES     += ../../../tests/utf8string.cpp

unix {
    LIBS += -lpfs
}

win32 {
    LIBS += pfs.lib
}
