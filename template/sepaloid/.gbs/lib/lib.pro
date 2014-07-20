include($$(GBS_HOME)/qmake/common-dll.pri)
include(../common.pri)

TARGET = $$GBS_TARGET_NAME

HEADERS += ../../include/*.hpp
SOURCES += ../../src/lib/*.cpp

INCLUDEPATH += ../../../cwt-io/include

unix {
    LIBS += -lcwt-io
}

win32 {
}
