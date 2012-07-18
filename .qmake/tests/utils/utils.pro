win32: include(/qmake/common-app.pri)
unix: include($$(HOME)/ws/qmake/common-app.pri)

TARGET = test_utils

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/utils.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\utils.c
    LIBS += cwt.lib
}
