win32: include(/qmake/common-app.pri)
unix: include($$(HOME)/ws/qmake/common-app.pri)

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
