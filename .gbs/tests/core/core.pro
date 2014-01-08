CWT_CONFIG=debug

include($$(GBS_HOME)/common-test.pri)

TARGET = test_debby_sqlite3

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../../cwt-sys/include
INCLUDEPATH += ../../../../cwt-debby/include

SOURCES += ../../../tests/test.cpp

unix {
#    QMAKE_CXXFLAGS += -pg
#    LIBS += -pg
    LIBS += -lpfs
    LIBS += -lcwt-sys
    LIBS += -lcwt-debby
#    LIBS += -lcwt-json
}

win32 {
}
