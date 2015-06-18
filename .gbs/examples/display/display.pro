include($$(GBS_HOME)/qmake/common-app.pri)

TARGET = example_display

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../examples/display.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
}

win32 {
    LIBS += pfs.lib
    LIBS += pfs-sys.lib
}
