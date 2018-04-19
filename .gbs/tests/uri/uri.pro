include($$(GBS_HOME)/common-app.pri)

TARGET = test_uri

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    HEADERS += ../../src/core/*.h
    SOURCES += ../../../tests/uri.c

    LIBS += -lcwt
}

win32 {
    HEADERS += ..\\..\\src\\core\\*.h
    SOURCES += ..\\..\\..\\tests\\uri.c
    LIBS += cwt.lib
}
