CWT_CONFIG=debug

include($$(GBS_HOME)/common-app.pri)

TARGET = test

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../pfs/include
INCLUDEPATH += ../../../cwt-sys/include

SOURCES += ../../tests/test.cpp

unix {
}

win32 {
}
