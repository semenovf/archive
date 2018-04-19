include($$(GBS_HOME)/common-app.pri)

TARGET = test_utils

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/utils.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\utils.c
    LIBS += cwt.lib
}
