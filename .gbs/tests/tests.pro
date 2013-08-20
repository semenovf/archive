CWT_CONFIG=debug

include($$(GBS_HOME)/common-app.pri)

TARGET = test_debby_sqlite3

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include
INCLUDEPATH += ../../../cwt-core/include
INCLUDEPATH += ../../../cwt-debby/include

SOURCES += ../../tests/test.cpp

unix {
    LIBS += -lcwt
    LIBS += -lcwt-logger
    LIBS += -lcwt-io
    LIBS += -lcwt-uri
    LIBS += -lcwt-json
    LIBS += -lcwt-debby
}

win32 {
}
