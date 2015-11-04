include($$(GBS_HOME)/qmake/common-app.pri)
TARGET =

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
