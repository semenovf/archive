CWT_CONFIG=debug

include($$(GBS_HOME)/common-app-qt.pri)

TARGET = example_logger

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include

unix {
    SOURCES += ../../../examples/logger.cpp
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\examples\\logger.cpp
    LIBS += cwt.lib
}
