include($$(GBS_HOME)/common-app.pri)

TARGET = test_ddi

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/dbi/ddi.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\dbi\\ddi.c
    LIBS += cwt.lib
}
