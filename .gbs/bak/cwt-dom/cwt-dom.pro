include($$(GBS_HOME)/qmake/common-dll.pri)
TARGET = pfs-dom
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../pfs/include

SOURCES += ../../src/*.cpp

unix {
    LIBS += -lpfs
}

win32 {
    LIBS += pfs.lib
}
