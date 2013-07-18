CWT_CONFIG=debug
include($$(GBS_HOME)/common-dll.pri)
TARGET = cwt-io

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include
INCLUDEPATH += ../../../cwt-core/include

HEADERS += ../../include/cwt/*.hpp
HEADERS += ../../include/cwt/io/*.hpp
SOURCES += ../../src/*.cpp
SOURCES += ../../src/unix/*.cpp

unix {
    LIBS += -lcwt
}

win32 {
}
