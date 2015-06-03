include($$(GBS_HOME)/qmake/common-petaloid.pri)
include(../common.pri)

TARGET = $$GBS_TARGET_NAME-logger

SOURCES   += ../../src/logger/*.cpp

unix {
}

win32 {
}
