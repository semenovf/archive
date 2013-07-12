CWT_CONFIG=debug

include($$(GBS_HOME)/common-dll-qt.pri)

TARGET = cwt
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include

DEPENDPATH += ../../include/cwt
DEPENDPATH += ../../src/qt

HEADERS += ../../include/cwt/*.hpp
HEADERS += ../../include/cwt/*.h
HEADERS += ../../src/*.hpp
HEADERS += ../../src/qt/*.hpp
SOURCES += ../../src/*.cpp
SOURCES += ../../src/*.c
SOURCES += ../../src/qt/*.cpp

#HEADERS += ../../experimental/*.hpp
#SOURCES += ../../experimental/*.cpp

unix {
    DEPENDPATH += ../../src/unix
    SOURCES += ../../src/unix/*.cpp
    SOURCES += ../../src/unix/*.c
}

win32 {
    DEPENDPATH += ../../src/unix
    SOURCES += ../../src/win32/*.cpp
    LIBS += ws2_32.lib
}