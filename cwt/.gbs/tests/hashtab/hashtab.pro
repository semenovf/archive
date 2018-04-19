include($$(GBS_HOME)/common-app.pri)

TARGET = test_hashtab

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/hashtab.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\hashtab.c
    LIBS += cwt.lib
}
