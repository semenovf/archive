include($$(GBS_HOME)/common-app.pri)

TARGET = test_fsm_rep

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/fsm_rep.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\fsm_rep.c
    LIBS += cwt.lib
}
