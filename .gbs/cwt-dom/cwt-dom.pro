include($$(GBS_HOME)/qmake/common-dll.pri)
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
