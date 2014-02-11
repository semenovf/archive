include($$(GBS_HOME)/common-dll.pri)
TARGET = cwt-debby
INCLUDEPATH += ../../include
INCLUDEPATH += ../../include/hiberlite
INCLUDEPATH += ../../../pfs/include
INCLUDEPATH += ../../../cwt-sys/include

SOURCES += ../../src/*.cpp
SOURCES += ../../src/hiberlite/*.cpp

unix {
    LIBS += -lpfs
    LIBS += -lcwt-sys
}

win32 {
    LIBS += cwt.lib
}
