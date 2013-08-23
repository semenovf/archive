CWT_CONFIG=debug

include($$(GBS_HOME)/common-app.pri)

TARGET = test_debby_sqlite3

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include
INCLUDEPATH += ../../../cwt-core/include
INCLUDEPATH += ../../../cwt-debby/include
INCLUDEPATH += ../../../cwt-fs/include

SOURCES += ../../tests/test.cpp

unix {
#    QMAKE_CXXFLAGS += -pg
#    LIBS += -pg
    LIBS += -lcwt
    LIBS += -lcwt-logger
    LIBS += -lcwt-io
    LIBS += -lcwt-uri
    LIBS += -lcwt-fs
    LIBS += -lcwt-json
    LIBS += -lcwt-debby
}

win32 {
}
