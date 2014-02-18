include($$(GBS_HOME)/qmake/common-app.pri)
TARGET =
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../pfs/include
INCLUDEPATH += ../../../cwt-sys/include

SOURCES += ../../src/*.cpp

unix {
    LIBS += -lpfs
    LIBS += -lcwt-sys
}

win32 {
    LIBS += pfs.lib
    LIBS += cwt-sys.lib
}
