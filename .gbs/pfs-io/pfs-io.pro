include($$(GBS_HOME)/qmake/common-dll.pri)
TARGET = pfs-io

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../pfs/include
INCLUDEPATH += ../../../pfs-sys/include

HEADERS += ../../include/pfs/*.hpp
HEADERS += ../../include/pfs/io/*.hpp
SOURCES += ../../src/*.cpp

unix {
    SOURCES += ../../src/unix/*.cpp
    LIBS += -lpfs
    LIBS += -lpfs-sys
}

win32 {
}
