CWT_CONFIG=debug
include($$(GBS_HOME)/common-dll.pri)
TARGET = pfs
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include
INCLUDEPATH += ../../../cwt-core/include

SOURCES += ../../src/*.cpp

unix {
}

win32 {
}
