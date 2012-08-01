win32: include(/qmake/common-app.pri)
unix: include($$(HOME)/ws/qmake/common-app.pri)

TARGET = test_list

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/list.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\list.c
    LIBS += cwt.lib
}
