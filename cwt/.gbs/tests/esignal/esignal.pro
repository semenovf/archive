include($$(GBS_HOME)/common-app.pri)

TARGET = test_event_signal

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/event/esignal-posix.c
    LIBS += -lcwt
}

win32 {
#    SOURCES += ..\\..\\..\\tests\\event\\signal.c
#    LIBS += cwt.lib
}
