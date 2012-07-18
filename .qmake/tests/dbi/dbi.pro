win32: include(/qmake/common-app.pri)
unix: include($$(HOME)/ws/qmake/common-app.pri)



TARGET = test_dbi_mysql

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/dbi/mysql.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\dbi\\mysql.c
    LIBS += cwt.lib
}
