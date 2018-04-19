include($$(GBS_HOME)/common-app.pri)

TARGET = test_event_channel

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/event/channel.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\event\\channel.c
    LIBS += cwt.lib
}
