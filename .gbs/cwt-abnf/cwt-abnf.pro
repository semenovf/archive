CWT_CONFIG=debug

include($$(GBS_HOME)/common-dll.pri)

TARGET = cwt-abnf

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt/include

# comment this in production mode
DEFINES += CWT_USE_TESTS


unix {
    HEADERS += ../../include/cwt/*.h
    SOURCES += ../../src/*.c

#    LIBS += -ldl
    LIBS += -lcwt
}

win32 {
    # see cwt/src/stdio.c for description
    DEFINES += _CRT_NON_CONFORMING_SWPRINTFS

    # compile as C sources
    QMAKE_CFLAGS += /TC
    QMAKE_CXXFLAGS += /TC

    HEADERS += ..\\..\\include\\cwt\\*.h
    SOURCES += ..\\..\\src\\*.c

#    LIBS += Advapi32.lib
#    LIBS += user32.lib
#    LIBS += ws2_32.lib
#    LIBS += iphlpapi.lib
}
