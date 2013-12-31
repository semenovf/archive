CWT_CONFIG=debug

include($$(GBS_HOME)/common-test.pri)

TARGET = test_dl

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../../cwt-sys/include

SOURCES += ../../../tests/dl.cpp

unix {
    LIBS += -lcwt-sys
}

win32 {
    LIBS += cwt-sys.lib
}
