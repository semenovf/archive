CWT_CONFIG=debug

include($$(GBS_HOME)/common-dll-qt.pri)

TARGET = cwt
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include

unix {
    HEADERS += ../../include/cwt/*.hpp
    HEADERS += ../../include/cwt/*.h
    SOURCES += ../../src/*.cpp
    SOURCES += ../../src/*.c
    SOURCES += ../../src/qt/*.cpp
    SOURCES += ../../src/unix/*.cpp
}

win32 {
    HEADERS += ..\\..\\include\\cwt\\*.hpp
    HEADERS += ..\\..\\include\\cwt\\*.h
    SOURCES += ..\\..\\src\\*.cpp
    SOURCES += ..\\..\\src\\*.c
    SOURCES += ..\\..\\src\\qt\\*.cpp
    SOURCES += ..\\..\\src\\win32\\*.cpp
}
