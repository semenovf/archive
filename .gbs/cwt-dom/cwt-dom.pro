CWT_CONFIG=debug
include($$(GBS_HOME)/common-dll.pri)
TARGET = cwt-dom
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../pfs/include

SOURCES += ../../src/*.cpp

unix {
    LIBS += -lpfs
}

win32 {
    LIBS += pfs.lib
}
