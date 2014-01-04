CWT_CONFIG=debug
include($$(GBS_HOME)/common-dll.pri)

TARGET = cwt-sys

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../pfs/include

HEADERS += ../../pcre8/*.h
SOURCES += ../../src/*.cpp
SOURCES += ../../pcre8/*.c

# PCRE configuration
DEFINES += HAVE_CONFIG_H

unix {
    SOURCES += ../../src/unix/*.cpp
    LIBS += -lpfs
    LIBS += -ldl
    LIBS += -lpthread
}

win32 {
    SOURCES += ..\\..\\src\\win32\\*.cpp
    LIBS += pfs.lib
}
