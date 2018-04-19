include($$(GBS_HOME)/common-app.pri)

TARGET = test_udp

INCLUDEPATH += ../../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../../tests/net/udp.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\..\\tests\\net\\udp.c
    LIBS += cwt.lib
}
