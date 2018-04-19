include($$(GBS_HOME)/common-app.pri)

TARGET = test_local

INCLUDEPATH += ../../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../../tests/net/local.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\..\\tests\\net\\local.c
    LIBS += cwt.lib
}
