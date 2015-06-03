include($$(GBS_HOME)/qmake/common-petaloid-qt-gui.pri)
include(../common.pri)

TARGET = $$GBS_TARGET_NAME-ui

SOURCES   += ../../src/ui/qt/*.cpp

unix {
}

win32 {
}
