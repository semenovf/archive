include($$(GBS_HOME)/qmake/common-petaloid.pri)
include(../common.pri)

TARGET = $$GBS_TARGET_NAME-db

HEADERS   += ../../src/db/*.hpp
SOURCES   += ../../src/db/*.cpp

unix {
}

win32 {
}
