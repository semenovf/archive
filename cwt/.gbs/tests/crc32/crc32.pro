include($$(GBS_HOME)/common-app.pri)

TARGET = test_crc32

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/crc32.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\crc32.c
    LIBS += cwt.lib
}
