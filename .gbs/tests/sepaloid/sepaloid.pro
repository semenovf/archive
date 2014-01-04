CWT_CONFIG=debug
include($$(GBS_HOME)/common-test.pri)
TARGET = test_sepaloid

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../../cwt-sys/include
INCLUDEPATH += ../../../../cwt-app/include

SOURCES += ../../../tests/sepaloid.cpp

unix {
    LIBS += -lpfs
    LIBS += -lcwt-sys
#    LIBS += -lcwt-json
#    LIBS += -lcwt-io
    LIBS += -lcwt-app
}

win32 {
}
