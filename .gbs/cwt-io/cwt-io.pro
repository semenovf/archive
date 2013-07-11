CWT_CONFIG=debug
include($$(GBS_HOME)/common-dll.pri)
TARGET = cwt-io

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include
INCLUDEPATH += ../../../cwt-core/include

DEPENDPATH += ../../include/cwt

unix {
    SOURCES += ../../src/*.cpp
    LIBS += -lcwt
}

win32 {
}
