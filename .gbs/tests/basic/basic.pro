

include($$(GBS_HOME)/common-test.pri)

TARGET = test_io

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../../cwt-sys/include
INCLUDEPATH += ../../../../cwt-io/include

HEADERS += ../../../include/cwt/io/*.hpp
SOURCES += ../../../tests/test.cpp

unix {
    LIBS += -lpfs
    LIBS += -lcwt-sys
    LIBS += -lcwt-io
}

win32 {
}
