CWT_CONFIG=debug
include($$(GBS_HOME)/common-dll.pri)
TARGET = pfs
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include
INCLUDEPATH += ../../../cwt-core/include

unix {
    SOURCES += ../../src/*.cpp
}

win32 {
    SOURCES += ..\\..\\src\\*.cpp
}
