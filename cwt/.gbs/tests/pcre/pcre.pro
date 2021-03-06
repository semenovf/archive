include($$(GBS_HOME)/common-app.pri)

TARGET = test_pcre

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/pcre/pcretest.c
    LIBS += -lcwt
    LIBS += -lcwt-pcre
}

win32 {
    SOURCES += ..\\..\\..\\tests\\pcre\\pcretest.c
    LIBS += cwt.lib
    LIBS += cwt-pcre.lib
}
