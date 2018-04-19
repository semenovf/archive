CWT_CONFIG=debug

include($$(GBS_HOME)/common-dll.pri)

TARGET = cwt
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include

HEADERS += ../../include/cwt/*.hpp
HEADERS += ../../include/cwt/*.h
HEADERS += ../../src/*.hpp
HEADERS += ../../pcre8/*.h
SOURCES += ../../src/*.cpp
SOURCES += ../../src/*.c
SOURCES += ../../pcre8/*.c
#SOURCES += ../../src/qt/*.cpp

# PCRE configuration
DEFINES += HAVE_CONFIG_H

unix {
    SOURCES += ../../src/unix/*.cpp
    SOURCES += ../../src/unix/*.c
    LIBS += -ldl -lpthread
}

win32 {
    SOURCES += ../../src/win32/*.cpp
    LIBS += ws2_32.lib
}