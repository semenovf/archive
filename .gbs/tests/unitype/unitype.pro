CWT_CONFIG=debug

include($$(GBS_HOME)/common-app.pri)

TARGET = test_unitype

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/unitype.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\unitype.c
    LIBS += cwt.lib
}
