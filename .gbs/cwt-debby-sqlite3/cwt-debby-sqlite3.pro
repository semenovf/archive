CWT_CONFIG=debug
include($$(GBS_HOME)/common-dll.pri)
TARGET = cwt-debby-sqlite3
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include
INCLUDEPATH += ../../../cwt-core/include
INCLUDEPATH += ../../../cwt-fs/include
INCLUDEPATH += ../../../cwt-debby/include

HEADERS += ../../sqlite3/*.h
SOURCES += ../../sqlite3/*.c
SOURCES += ../../src/*.cpp

unix {
#    QMAKE_CXXFLAGS += -pg
#    LIBS += -pg
    LIBS += -lcwt
    LIBS += -lcwt-fs
}

win32 {
    # see cwt/src/stdio.c for description
    DEFINES += _CRT_NON_CONFORMING_SWPRINTFS

    # compile as C sources
    QMAKE_CFLAGS += /TC
    QMAKE_CXXFLAGS += /TC
}
