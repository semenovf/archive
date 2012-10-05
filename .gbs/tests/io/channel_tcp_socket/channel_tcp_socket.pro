include($$(GBS_HOME)/common-app.pri)

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
