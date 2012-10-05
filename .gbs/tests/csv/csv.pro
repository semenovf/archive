include($$(GBS_HOME)/common-app.pri)

TARGET = test_csv

INCLUDEPATH += ../../../include
DEFINES += CWT_USE_TESTS

unix {
    SOURCES += ../../../tests/csv.c
    LIBS += -lcwt
    LIBS += -lcwt-csv
}

win32 {
    SOURCES += ..\\..\\..\\tests\\csv.c
    LIBS += cwt.lib
    LIBS += cwt-csv.lib
}
