CWT_CONFIG=debug

include($$(GBS_HOME)/common-app.pri)

TARGET = example_logger

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../../pfs-unitype/include
INCLUDEPATH += ../../../../cwt-sys/include

SOURCES += ../../../examples/logger.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-unitype
    LIBS += -lcwt-sys
}

win32 {
    LIBS += pfs.lib
    LIBS += pfs-unitype.lib
    LIBS += cwt-sys.lib
}
