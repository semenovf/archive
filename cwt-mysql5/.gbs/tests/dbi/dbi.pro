include($$(GBS_HOME)/common-app.pri)

TARGET = test_dbi_mysql

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt/include

DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/mysql.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\mysql.c
    LIBS += cwt.lib
}
