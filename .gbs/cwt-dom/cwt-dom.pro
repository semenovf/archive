CWT_CONFIG=debug
include($$(GBS_HOME)/common-dll.pri)
TARGET = cwt-dom
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../pfs/include
INCLUDEPATH += ../../../pfs-unitype/include

SOURCES += ../../src/*.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-unitype
}

win32 {
    LIBS += pfs.lib
    LIBS += pfs-unitype.lib
}
