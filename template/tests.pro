include($$(GBS_HOME)/qmake/common-test.pri)
TARGET =
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../pfs/include
INCLUDEPATH += ../../../pfs-pfs/include

SOURCES += ../../tests/test.cpp

unix {
}

win32 {
}
