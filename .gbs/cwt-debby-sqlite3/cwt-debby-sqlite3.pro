include($$(GBS_HOME)/common-dll.pri)
TARGET = cwt-debby-sqlite3
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../pfs/include
INCLUDEPATH += ../../../cwt-sys/include
INCLUDEPATH += ../../../cwt-debby/include
#INCLUDEPATH += ../../../cwt-json/include

HEADERS += ../../sqlite3/*.h
HEADERS += ../../src/*.hpp
SOURCES += ../../sqlite3/*.c
SOURCES += ../../src/*.cpp

unix {
#    QMAKE_CFLAGS += -pg
#    QMAKE_CXXFLAGS += -pg
    LIBS += -lpfs
    LIBS += -lcwt-sys
}

win32 {
    # see cwt/src/stdio.c for description
    DEFINES += _CRT_NON_CONFORMING_SWPRINTFS

    # compile as C sources
    QMAKE_CFLAGS += /TC
    QMAKE_CXXFLAGS += /TC
}
