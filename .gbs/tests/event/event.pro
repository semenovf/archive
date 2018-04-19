include($$(GBS_HOME)/common-app.pri)

TARGET = test_event

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/event/event.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\event\\event.c
    LIBS += cwt.lib
}
