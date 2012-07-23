win32: include(/qmake/common-app.pri)
unix: include($$(HOME)/ws/qmake/common-app.pri)


TARGET = test_bytearr

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/bytearr.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\bytearr.c
    LIBS += cwt.lib
}
