include($$(GBS_HOME)/qmake/common-dll.pri)
TARGET = pfs-io

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../pfs/include
INCLUDEPATH += ../../../pfs-sys/include

SOURCES += ../../src/*.cpp

unix {
    SOURCES += ../../src/unix/*.cpp
    LIBS += -lpfs
    LIBS += -lpfs-sys
}

win32 {
    SOURCES += ..\\..\\src\\win\\*.cpp
    LIBS += pfs.lib
    LIBS += pfs-sys.lib
}

HEADERS += ../../include/*
include($$(GBS_HOME)/qmake/deploy.pri)
