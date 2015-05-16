include($$(GBS_HOME)/qmake/common-dll.pri)
TARGET = pfs-app
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../pfs/include
INCLUDEPATH += ../../../pfs-sys/include

HEADERS += ../../include/pfs/*.hpp

unix {
    SOURCES += ../../src/*.cpp
    LIBS += -lpfs
    LIBS += -lpfs-sys
}

win32 {
    SOURCES += ..\\..\\src\\*.cpp
    LIBS += lpfs.lib
    LIBS += lpfs-sys.lib
}
