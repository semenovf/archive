win32: include(/qmake/common-app.pri)
unix: include($$(HOME)/ws/qmake/common-app.pri)

TARGET = test_options

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/options.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\options.c
    LIBS += cwt.lib
}
