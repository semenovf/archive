include($$(GBS_HOME)/qmake/common-petaloid.pri)
include(../common.pri)

TARGET = $$GBS_TARGET_NAME-io

SOURCES += ../../src/io/*.cpp

unix {
}

win32 {
}
