include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_dl

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../../cwt-sys/include

SOURCES += ../../../tests/dl.cpp

unix {
    LIBS += -lpfs
    LIBS += -lcwt-sys
}

win32 {
    LIBS += cwt-sys.lib
}
