include($$(GBS_HOME)/common-app.pri)

TARGET = test_crc64

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/crc64.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\crc64.c
    LIBS += cwt.lib
}
