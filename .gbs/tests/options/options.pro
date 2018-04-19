include($$(GBS_HOME)/common-app.pri)

TARGET = test_options

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/options.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\options.c
    LIBS += cwt.lib
}
