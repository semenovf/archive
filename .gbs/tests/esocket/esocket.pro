include($$(GBS_HOME)/common-app.pri)

TARGET = test_event_socket

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/event/socket.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\event\\socket.c
    LIBS += cwt.lib
}
