CWT_CONFIG=debug

include($$(GBS_HOME)/common-dll.pri)

TARGET = cwt-sqlite3

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt/include

# comment this in production mode
DEFINES += CWT_USE_TESTS


unix {
    HEADERS += ../../include/sqlite3/*.h
    HEADERS += ../../include/cwt/*.h
    SOURCES += ../../sqlite3/*.c
    SOURCES += ../../src/*.c

    LIBS += -lcwt
}

win32 {
    # see cwt/src/stdio.c for description
    DEFINES += _CRT_NON_CONFORMING_SWPRINTFS

    # compile as C sources
    QMAKE_CFLAGS += /TC
    QMAKE_CXXFLAGS += /TC

    HEADERS += ..\\..\\include\\sqlite3\\*.h
    HEADERS += ..\\..\\include\\cwt\\*.h
    SOURCES += ..\\..\\sqlite3\\*.c
    SOURCES += ..\\..\\src\\*.c
}
