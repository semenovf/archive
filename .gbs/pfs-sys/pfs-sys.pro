include($$(GBS_HOME)/qmake/common-dll.pri)

TARGET = pfs-sys

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../pfs/include

# PCRE configuration
DEFINES += HAVE_CONFIG_H
DEFINES += PCRE_EXP_DECL=extern
DEFINES += PCRE_EXP_DEFN=
DEFINES += PCRE_EXP_DATA_DEFN=

unix {
    # Avoid warning:
    # ... implicit declaration of function 'write' (read/lseek/close)
    # while compiling 'zlib'
    QMAKE_CFLAGS += -include unistd.h

    SOURCES += ../../src/*.cpp
 #   SOURCES += ../../src/*.c
    SOURCES += ../../pcre8/*.c
    SOURCES += ../../zlib/*.c
    SOURCES += ../../src/unix/*.cpp
    SOURCES += ../../src/x11/*.cpp

    LIBS += -lpfs
    LIBS += -ldl
    LIBS += -lpthread
    LIBS += -lX11
}

win32 {
    SOURCES += ..\\..\\src\\*.cpp
#    SOURCES += ..\\..\\src\\*.c
    SOURCES += ..\\..\\src\\win\\*.cpp
    SOURCES += ..\\..\\pcre8\\*.c
    SOURCES += ..\\..\\zlib\\*.c
    LIBS += pfs.lib
}

HEADERS += ../../include/*
include($$(GBS_HOME)/qmake/deploy.pri)
