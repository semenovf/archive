win32: include(/qmake/common-app.pri)
unix: include($$(HOME)/ws/qmake/common-app.pri)


TARGET = test_ini

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/ini.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\ini.c
    LIBS += cwt.lib
}
