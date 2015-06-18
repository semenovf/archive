include($$(GBS_HOME)/qmake/common-dll.pri)
TARGET = pfs-app

INCLUDEPATH = $$prependIncludePath(../../include)

unix {
    SOURCES += ../../src/*.cpp
    SOURCES += ../../src/unix/*.cpp
    LIBS += -lpfs
    LIBS += -lpfs-sys
    LIBS += -lpfs-settings
}

win32 {
    SOURCES += ..\\..\\src\\*.cpp
    SOURCES += ..\\..\\src\\win\\*.cpp
    LIBS += pfs.lib
    LIBS += pfs-sys.lib
    LIBS += pfs-settings.lib
}

HEADERS += ../../include/pfs/
include($$(GBS_HOME)/qmake/deploy.pri)
