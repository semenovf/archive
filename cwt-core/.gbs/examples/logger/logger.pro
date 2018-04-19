CWT_CONFIG=debug

include($$(GBS_HOME)/common-app.pri)

TARGET = example_logger

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include

SOURCES += ../../../examples/logger.cpp

unix {
    LIBS += -lcwt
}

win32 {
}
