win32: include(/qmake/common-app.pri)
unix: include($$(HOME)/ws/qmake/common-app.pri)

TARGET = test_tcp

INCLUDEPATH += ../../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../../tests/net/tcp.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\..\\tests\\net\\tcp.c
    LIBS += cwt.lib
}
