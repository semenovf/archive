include($$(GBS_HOME)/common-app.pri)

TARGET = test_fsm_rpt

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/fsm_rpt.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\fsm_rpt.c
    LIBS += cwt.lib
}
