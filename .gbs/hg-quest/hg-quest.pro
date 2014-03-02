include($$(GBS_HOME)/qmake/common-app-qt.pri)
include(../common.pri)

TARGET = hg-quest

QT += network

HEADERS += ../../include/hg/quest/*.hpp
SOURCES += ../../src/*.cpp

unix {
}

win32 {
}
