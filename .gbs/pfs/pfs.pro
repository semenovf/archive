CWT_CONFIG=debug
include($$(GBS_HOME)/common-dll.pri)
TARGET = pfs
INCLUDEPATH += ../../include

unix {
    SOURCES += ../../src/*.cpp
}

win32 {
    SOURCES += ..\\..\\src\\*.cpp
}
