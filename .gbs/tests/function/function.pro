include($$(GBS_HOME)/common-test.pri)

TARGET = test_function

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
#INCLUDEPATH += ../../../../cwt-test/include

SOURCES += ../../../tests/function.cpp

unix {
    LIBS += -lpfs
}

win32 {
    LIBS += pfs.lib
}
