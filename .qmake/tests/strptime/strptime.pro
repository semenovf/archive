win32: include(/qmake/common-app.pri)
unix: include($$(HOME)/ws/qmake/common-app.pri)

TARGET = test_strptime

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/strptime.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\strptime.c
    LIBS += cwt.lib
}
