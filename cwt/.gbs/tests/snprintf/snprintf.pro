include($$(GBS_HOME)/common-app.pri)

TARGET = test_snprintf

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    HEADERS += ../../src/core/*.h
    SOURCES += ../../../tests/snprintf.c

    LIBS += -lcwt
}

win32 {
    HEADERS += ..\\..\\src\\core\\*.h
    SOURCES += ..\\..\\..\\tests\\snprintf.c
    LIBS += cwt.lib
}
