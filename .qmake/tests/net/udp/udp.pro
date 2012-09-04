win32: include(/qmake/common-app.pri)
unix: include($$(HOME)/ws/qmake/common-app.pri)

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
