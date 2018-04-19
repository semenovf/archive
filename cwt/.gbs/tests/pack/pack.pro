include($$(GBS_HOME)/common-app.pri)

TARGET = test_pack

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/pack.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\pack.c
    LIBS += cwt.lib
}
