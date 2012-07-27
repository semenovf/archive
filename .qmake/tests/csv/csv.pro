win32: include(/qmake/common-app.pri)
unix: include($$(HOME)/ws/qmake/common-app.pri)


TARGET = test_csv

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/csv.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\csv.c
    LIBS += cwt.lib
}
