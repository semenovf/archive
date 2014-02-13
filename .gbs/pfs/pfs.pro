include($$(GBS_HOME)/qmake/common-dll.pri)
TARGET = pfs
INCLUDEPATH += ../../include

unix {
    SOURCES += ../../src/*.cpp
}

win32 {
    SOURCES += ..\\..\\src\\*.cpp
}
