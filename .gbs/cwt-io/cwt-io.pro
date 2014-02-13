include($$(GBS_HOME)/qmake/common-dll.pri)
TARGET = cwt-io

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../pfs/include
INCLUDEPATH += ../../../cwt-sys/include

HEADERS += ../../include/cwt/*.hpp
HEADERS += ../../include/cwt/io/*.hpp
SOURCES += ../../src/*.cpp

unix {
    SOURCES += ../../src/unix/*.cpp
    LIBS += -lpfs
    LIBS += -lcwt-sys
}

win32 {
}
