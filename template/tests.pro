CWT_CONFIG=debug

include($$(GBS_HOME)/common-app.pri)

TARGET = test

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include

unix {
    SOURCES += ../../tests/test.cpp
}

win32 {
    SOURCES += ..\\..\\tests\\test.cpp
}
