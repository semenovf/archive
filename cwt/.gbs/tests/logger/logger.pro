include($$(GBS_HOME)/common-app.pri)

TARGET = test_logger

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/logger.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\logger.c
    LIBS += cwt.lib
}
