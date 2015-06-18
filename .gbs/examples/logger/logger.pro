include($$(GBS_HOME)/qmake/common-app.pri)

TARGET = example_logger

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../examples/logger.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
}

win32 {
    LIBS += pfs.lib
    LIBS += pfs-sys.lib
}
