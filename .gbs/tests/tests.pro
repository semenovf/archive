CWT_CONFIG=debug

include($$(GBS_HOME)/common-app.pri)

TARGET = test

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include
INCLUDEPATH += ../../../cwt-core/include

SOURCES += ../../tests/test.cpp

unix {
}

win32 {
}
