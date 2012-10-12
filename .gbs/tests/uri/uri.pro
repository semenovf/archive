include($$(GBS_HOME)/common-app.pri)

TARGET = test_uri

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/uri.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\uri.c
    LIBS += cwt.lib
}
