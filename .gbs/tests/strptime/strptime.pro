include($$(GBS_HOME)/common-app.pri)

TARGET = test_strptime

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/strptime.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\strptime.c
    LIBS += cwt.lib
}
