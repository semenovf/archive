CWT_CONFIG=debug

include($$(GBS_HOME)/common-app.pri)

TARGET = test_io

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include
INCLUDEPATH += ../../../cwt-core/include

DEPENDPATH += ../../include/cwt
DEPENDPATH += ../../include/cwt/io

unix {
    SOURCES += ../../tests/test.cpp
    LIBS += -lcwt
    LIBS += -lcwt-io
}

win32 {
    SOURCES += ..\\..\\tests\\test.cpp
}
