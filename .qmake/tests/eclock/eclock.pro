include(/qmake/common-app.pri)

TARGET = test_event_clock

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/event/clock.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\event\\clock.c
    LIBS += cwt.lib
}
