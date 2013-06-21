CWT_CONFIG=debug

include($$(GBS_HOME)/common-dll-qt.pri)

TARGET = cwt
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include

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
    HEADERS += ../../src/unix/*.hpp
    SOURCES += ../../src/unix/*.cpp
    SOURCES += ../../src/unix/*.c
}

win32 {}