win32: include(/qmake/common-app.pri)
unix: include($$(HOME)/ws/qmake/common-app.pri)

TARGET = test_ringbuf

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/ringbuf.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\ringbuf.c
    LIBS += cwt.lib
}
