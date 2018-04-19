include($$(GBS_HOME)/qmake/common-dll.pri)
TARGET=cwt-abnf
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include
INCLUDEPATH += ../../../cwt-core/include

HEADERS += ../../include/cwt/*.hpp
SOURCES += ../../src/*.cpp

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
