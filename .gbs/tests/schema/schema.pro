CWT_CONFIG=debug

include($$(GBS_HOME)/common-app.pri)

TARGET = test_debby_schema

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include
INCLUDEPATH += ../../../../cwt-io/include
INCLUDEPATH += ../../../../cwt-csv/include

SOURCES += ../../../tests/test_schema.cpp

unix {
    LIBS += -lcwt
    LIBS += -lcwt-io
    LIBS += -lcwt-csv
    LIBS += -lcwt-debby
}

win32 {
}
