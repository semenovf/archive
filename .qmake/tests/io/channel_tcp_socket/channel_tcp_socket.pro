win32: include(/qmake/common-app.pri)
unix: include($$(HOME)/ws/qmake/common-app.pri)

TARGET = test_channel_tcp_socket

INCLUDEPATH += ../../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../../tests/io/channel_tcp_socket.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\..\\tests\\io\\channel_tcp_socket.c
    LIBS += cwt.lib
}
