

include($$(GBS_HOME)/common-app.pri)

TARGET = example_logger

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../../cwt-sys/include

SOURCES += ../../../examples/logger.cpp

unix {
    LIBS += -lpfs
    LIBS += -lcwt-sys
}

win32 {
    LIBS += pfs.lib
    LIBS += cwt-sys.lib
}
