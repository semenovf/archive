include($$(GBS_HOME)/qmake/common-petaloid.pri)
include(../common.pri)

TARGET = $$GBS_TARGET_NAME-prefs

HEADERS   += ../../src/prefs/*.hpp
SOURCES   += ../../src/prefs/*.cpp

unix {
}

win32 {
}
