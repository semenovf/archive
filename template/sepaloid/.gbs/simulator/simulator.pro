include($$(GBS_HOME)/qmake/common-petaloid.pri)
include(../common.pri)

TARGET = $$GBS_TARGET_NAME-simulator

SOURCES += ../../src/simulator/*.cpp

unix {}

win32 {}

include($$(GBS_HOME)/qmake/deploy.pri)