CWT_CONFIG=debug

win32: include(/qmake/common-app.pri)
unix: include($$(HOME)/ws/qmake/common-app.pri)

TARGET = test_unitype

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/unitype.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\unitype.c
    LIBS += cwt.lib
}
