CWT_CONFIG=debug

include($$(GBS_HOME)/common-dll.pri)

TARGET = cwt
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include

HEADERS += ../../include/cwt/*.hpp
HEADERS += ../../include/cwt/*.h
HEADERS += ../../src/*.hpp
SOURCES += ../../src/*.cpp
SOURCES += ../../src/*.c
#SOURCES += ../../src/qt/*.cpp

unix {
    SOURCES += ../../src/unix/*.cpp
    SOURCES += ../../src/unix/*.c
    LIBS += -ldl -lpthread
}

win32 {
    SOURCES += ../../src/win32/*.cpp
    LIBS += ws2_32.lib
}