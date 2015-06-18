include($$(GBS_HOME)/qmake/common-dll.pri)
TARGET = pfs
INCLUDEPATH = $$prependIncludePath(../../include)

unix {
    SOURCES += ../../src/*.cpp
    SOURCES += ../../src/*.c
}

win32 {
    SOURCES += ..\\..\\src\\*.cpp
    SOURCES += ..\\..\\src\\*.c
}

HEADERS += ../../include/*.h
HEADERS += ../../include/*.hpp
HEADERS += ../../include/pfs
include($$(GBS_HOME)/qmake/deploy.pri)
