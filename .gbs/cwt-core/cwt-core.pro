CWT_CONFIG=debug

include($$(GBS_HOME)/common-dll-qt.pri)

TARGET = cwt
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include

HEADERS += ../../include/cwt/*.hpp
HEADERS += ../../include/cwt/*.h
SOURCES += ../../src/*.cpp
SOURCES += ../../src/*.c
SOURCES += ../../src/qt/bytearray_qt.cpp
SOURCES += ../../src/qt/string_qt.cpp
SOURCES += ../../src/unix/*.cpp
SOURCES += ../../src/unix/*.c

#HEADERS += ../../experimental/*.hpp
#SOURCES += ../../experimental/*.cpp

unix {}

win32 {}