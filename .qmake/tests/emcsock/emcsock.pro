include(/qmake/common-app.pri)

TARGET = test_event_mcsocket

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/event/mcsocket.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\event\\mcsocket.c
    LIBS += cwt.lib
}
