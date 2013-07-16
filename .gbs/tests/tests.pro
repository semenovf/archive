CWT_CONFIG=debug

include($$(GBS_HOME)/common-app.pri)

TARGET = test_io

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include
INCLUDEPATH += ../../../cwt-core/include

DEPENDPATH += ../../include/cwt
DEPENDPATH += ../../include/cwt/io

unix {
    HEADERS += ../../include/cwt/io
    SOURCES += ../../tests/test.cpp
    LIBS += -lcwt-io
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\tests\\test.cpp
}
