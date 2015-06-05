include($$(GBS_HOME)/qmake/common-petaloid-qt-gui.pri)
include(../common.pri)

TARGET = $$GBS_TARGET_NAME-ui

SOURCES += ../../src/ui/qt/*.cpp
#
# For generate moc_* files
#
HEADERS += ../../src/ui/qt/*.hpp

unix {}

win32 {}

include($$(GBS_HOME)/qmake/deploy.pri)