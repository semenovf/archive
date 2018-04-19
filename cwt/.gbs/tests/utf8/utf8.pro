include($$(GBS_HOME)/common-app.pri)

TARGET = test_utf8

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/utf8.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\utf8.c
    LIBS += cwt.lib
}
