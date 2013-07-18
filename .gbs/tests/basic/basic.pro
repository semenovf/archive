CWT_CONFIG=debug

include($$(GBS_HOME)/common-app.pri)

TARGET = test_io

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include
INCLUDEPATH += ../../../../cwt-io/include

HEADERS += ../../../include/cwt/io/*.hpp
SOURCES += ../../../tests/test.cpp

unix {
    LIBS += -lcwt-io
    LIBS += -lcwt
}

win32 {
}
