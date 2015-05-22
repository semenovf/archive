include($$(GBS_HOME)/qmake/common-dll.pri)
include(../common.pri)

TARGET = $$GBS_TARGET_NAME

HEADERS += ../../include/*.hpp
SOURCES += ../../src/lib/*.cpp

INCLUDEPATH += ../../../pfs-io/include

unix {
    LIBS += -lpfs-io
}

win32 {
}
