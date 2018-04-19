include($$(GBS_HOME)/common-app.pri)

TARGET = test_channel

INCLUDEPATH += ../../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../../tests/io/channel.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\..\\tests\\io\\channel.c
    LIBS += cwt.lib
}
