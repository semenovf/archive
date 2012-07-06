include(/qmake/common-app.pri)

TARGET = test_strutils

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/strutils.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\strutils.c
    LIBS += cwt.lib
}
