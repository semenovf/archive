include($$(GBS_HOME)/qmake/common-dll.pri)
TARGET = pfs
INCLUDEPATH += ../../include

unix {
    SOURCES += ../../src/*.cpp
    SOURCES += ../../src/*.c
}

win32 {
    SOURCES += ..\\..\\src\\*.cpp
    SOURCES += ..\\..\\src\\*.c
}

HEADERS += ../../include/*
include($$(GBS_HOME)/qmake/deploy.pri)
