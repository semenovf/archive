CWT_CONFIG=debug
include($$(GBS_HOME)/common-dll.pri)
TARGET = cwt-io

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include
INCLUDEPATH += ../../../cwt-core/include

DEPENDPATH += ../../include/cwt
DEPENDPATH += ../../include/cwt/io

unix {
    HEADERS += ../../include/cwt/*.hpp
    HEADERS += ../../include/cwt/io/*.hpp
    SOURCES += ../../src/unix/*.cpp
    LIBS += -lcwt
}

win32 {
}
