include($$(GBS_HOME)/qmake/common-petaloid.pri)
include(../common.pri)

TARGET = $$GBS_TARGET_NAME-core

SOURCES   += ../../src/core/*.cpp

unix {
}

win32 {
}
