CWT_CONFIG=debug

include($$(GBS_HOME)/common-app.pri)

TARGET = test_debby_schema

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include
INCLUDEPATH += ../../../../cwt-json/include

SOURCES += ../../../tests/schema.cpp

unix {
    LIBS += -lcwt
    LIBS += -lcwt-debby
}

win32 {
}
