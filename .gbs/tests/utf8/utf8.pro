CWT_CONFIG=debug

include($$(GBS_HOME)/common-test.pri)

TARGET = test_io_utf8

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../../cwt-sys/include
INCLUDEPATH += ../../../../cwt-io/include

HEADERS += ../../../include/cwt/io/*.hpp
SOURCES += ../../../tests/utf8.cpp

unix {
    LIBS += -lcwt-io
}

win32 {
}
