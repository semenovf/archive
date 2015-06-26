include($$(GBS_HOME)/qmake/common-test.pri)
TARGET =

INCLUDEPATH = $$prependIncludePath(../../include)
SOURCES += ../../tests/test.cpp

unix {
}

win32 {
}
