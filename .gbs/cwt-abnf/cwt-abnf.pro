CWT_CONFIG=debug
include($$(GBS_HOME)/common-dll-qt.pri)
TARGET=cwt-abnf
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include
INCLUDEPATH += ../../../cwt-core/include

unix {
    HEADERS += ../../include/cwt/*.hpp
    SOURCES += ../../src/*.cpp
    LIBS += -lcwt
}

win32 {
    HEADERS += ..\\..\\include\\cwt\\*.hpp
    SOURCES += ..\\..\\src\\*.cpp
    LIBS += cwt.lib
}
