include($$(GBS_HOME)/common-app.pri)

TARGET = test_abnf

INCLUDEPATH += ../../../../cwt/include
INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/abnf.c
    LIBS += -lcwt
    LIBS += -lcwt-abnf
}

win32 {
    SOURCES += ..\\..\\..\\tests\\abnf.c
    LIBS += cwt.lib
    LIBS += cwt-abnf.lib
}
