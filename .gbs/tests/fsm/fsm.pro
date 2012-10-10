include($$(GBS_HOME)/common-app.pri)

TARGET = test_fsm

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/fsm.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\fsm.c
    LIBS += cwt.lib
}
