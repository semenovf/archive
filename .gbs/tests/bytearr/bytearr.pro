include($$(GBS_HOME)/common-app.pri)

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
