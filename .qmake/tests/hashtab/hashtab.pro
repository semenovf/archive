win32: include(/qmake/common-app.pri)
unix: include($$(HOME)/ws/qmake/common-app.pri)

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
