win32: include(/qmake/common-app.pri)
unix: include($$(HOME)/ws/qmake/common-app.pri)



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
