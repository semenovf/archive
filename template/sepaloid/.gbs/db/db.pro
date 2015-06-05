include($$(GBS_HOME)/qmake/common-petaloid.pri)
include(../common.pri)

TARGET = $$GBS_TARGET_NAME-db

SOURCES   += ../../src/db/*.cpp

unix {}

win32 {}

include($$(GBS_HOME)/qmake/deploy.pri)