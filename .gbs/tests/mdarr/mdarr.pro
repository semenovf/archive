include($$(GBS_HOME)/common-app.pri)

TARGET = test_mdarr

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/mdarr.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\mdarr.c
    LIBS += cwt.lib
}
