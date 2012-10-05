CWT_CONFIG=debug

include($$(GBS_HOME)/common-dll.pri)

TARGET = cwt

INCLUDEPATH += ../../include
#INCLUDEPATH += /include/mysql

# comment this in production mode
DEFINES += CWT_USE_TESTS


unix {
    HEADERS += ../../include/cwt/*.h
    HEADERS += ../../include/cwt/io/*.h
    HEADERS += ../../include/cwt/dbi/*.h
    HEADERS += ../../include/cwt/net/*.h
    HEADERS += ../../src/net/*.h

    SOURCES += ../../src/core/*.c
    SOURCES += ../../src/event/*.c
    SOURCES += ../../src/io/*.c
    SOURCES += ../../src/io/serial/*.c
    SOURCES += ../../src/io/serial/linux/*.c
    SOURCES += ../../src/dbi/*.c
    SOURCES += ../../src/net/*.c
    SOURCES += ../../src/net/unix/*.c

    LIBS += -ldl
#    LIBS += -lmysql
}

win32 {
    # see cwt/src/stdio.c for description
    DEFINES += _CRT_NON_CONFORMING_SWPRINTFS

    # compile as C sources
    QMAKE_CFLAGS += /TC
    QMAKE_CXXFLAGS += /TC


    HEADERS += ..\\..\\include\\cwt\\*.h
    HEADERS += ..\\..\\include\\cwt\\io\\*.h
    HEADERS += ..\\..\\include\\cwt\\dbi\\*.h
    HEADERS += ..\\..\\include\\cwt\\net\\*.h
    HEADERS += ..\\..\\src\\net\\*.h

    SOURCES += ..\\..\\src\\core\\*.c
    SOURCES += ..\\..\\src\\event\\*.c
    SOURCES += ..\\..\\src\\io\\*.c
    SOURCES += ..\\..\\src\\io\\serial\\*.c
    SOURCES += ..\\..\\src\\io\\serial\\win\\*.c
    SOURCES += ..\\..\\src\\dbi\\*.c
    SOURCES += ..\\..\\src\\net\\*.c
    SOURCES += ..\\..\\src\\net\\win\\*.c

    LIBS += Advapi32.lib
    LIBS += user32.lib
    LIBS += ws2_32.lib
    LIBS += iphlpapi.lib
}
