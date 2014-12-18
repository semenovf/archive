include($$(GBS_HOME)/qmake/common-test.pri)
TARGET =
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../pfs/include
INCLUDEPATH += ../../../cwt-sys/include

SOURCES += ../../tests/test.cpp

unix {
}

win32 {
}
