include($$(GBS_HOME)/common-app.pri)

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
