include($$(GBS_HOME)/common-app.pri)

TARGET = test_array

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/array.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\array.c
    LIBS += cwt.lib
}
