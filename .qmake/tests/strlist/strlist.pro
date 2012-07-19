win32: include(/qmake/common-app.pri)
unix: include($$(HOME)/ws/qmake/common-app.pri)

TARGET = test_strlist

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/strlist.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\strlist.c
    LIBS += cwt.lib
}
