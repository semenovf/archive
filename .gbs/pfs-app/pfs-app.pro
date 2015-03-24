include($$(GBS_HOME)/qmake/common-dll.pri)
TARGET = pfs-app
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../pfs/include
INCLUDEPATH += ../../../cwt-sys/include

HEADERS += ../../include/pfs/*.hpp
SOURCES += ../../src/*.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
}

win32 {
    LIBS += lpfs.lib
    LIBS += lpfs-sys.lib
}
