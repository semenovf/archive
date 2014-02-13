include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_safeformat

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../../cwt-sys/include

SOURCES += ../../../tests/safeformat.cpp

unix {
    LIBS += -lpfs
    LIBS += -lcwt-sys
}

win32 {
    LIBS += pfs.lib
    LIBS += cwt-sys.lib
}
