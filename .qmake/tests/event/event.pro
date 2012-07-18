win32: include(/qmake/common-app.pri)
unix: include($$(HOME)/ws/qmake/common-app.pri)

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
