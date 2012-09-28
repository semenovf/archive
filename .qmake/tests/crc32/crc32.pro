win32: include(/qmake/common-app.pri)
unix: include($$(HOME)/ws/qmake/common-app.pri)


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
