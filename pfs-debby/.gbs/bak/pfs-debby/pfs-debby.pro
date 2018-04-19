include($$(GBS_HOME)/qmake/common-dll.pri)
TARGET = pfs-debby

INCLUDEPATH = $$prependIncludePath(../../include)

SOURCES += ../../src/*.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
}

win32 {
    LIBS += pfs.lib
    LIBS += pfs-sys.lib
}
