include($$(GBS_HOME)/common-app.pri)

TARGET = test_fsm_date

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/fsm_date.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\fsm_date.c
    LIBS += cwt.lib
}
