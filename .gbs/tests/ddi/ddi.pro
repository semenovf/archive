include($$(GBS_HOME)/common-app.pri)

TARGET = test_ddi

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt/include

DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/ddi.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\ddi.c
    LIBS += cwt.lib
}
