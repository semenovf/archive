CWT_CONFIG=debug

include($$(GBS_HOME)/common-app.pri)

TARGET = test_debby_schema

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../../cwt-sys/include
#INCLUDEPATH += ../../../../cwt-io/include
#INCLUDEPATH += ../../../../cwt-csv/include

SOURCES += ../../../tests/test_schema.cpp

unix {
    LIBS += -lpfs
    LIBS += -lcwt-sys
#    LIBS += -lcwt-csv
#    LIBS += -lcwt-debby
}

win32 {
}
