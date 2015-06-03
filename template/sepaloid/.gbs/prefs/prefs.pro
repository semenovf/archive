include($$(GBS_HOME)/qmake/common-petaloid.pri)
include(../common.pri)

TARGET = $$GBS_TARGET_NAME-prefs

SOURCES   += ../../src/prefs/*.cpp

unix {
}

win32 {
}
