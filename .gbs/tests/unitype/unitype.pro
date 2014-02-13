include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_unitype

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
#INCLUDEPATH += ../../../../cwt-test/include

SOURCES     += ../../../tests/unitype.cpp

unix {
    LIBS += -lpfs
}

win32 {
    LIBS += pfs.lib
}
