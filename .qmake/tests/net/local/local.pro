win32: include(/qmake/common-app.pri)
unix: include($$(HOME)/ws/qmake/common-app.pri)

TARGET = test_local

INCLUDEPATH += ../../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../../tests/net/local.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\..\\tests\\net\\local.c
    LIBS += cwt.lib
}
