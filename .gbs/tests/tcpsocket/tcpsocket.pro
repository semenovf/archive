CWT_CONFIG=debug

include($$(GBS_HOME)/common-test.pri)

TARGET = test_io_tcpsocket

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../../cwt-sys/include
INCLUDEPATH += ../../../../cwt-io/include

SOURCES += ../../../tests/tcpsocket.cpp
HEADERS += ../../../include/cwt/io/*.hpp
HEADERS += ../../../include/cwt/net/*.hpp

unix {
    LIBS += -lpfs
    LIBS += -lcwt-sys
    LIBS += -lcwt-io
}

win32 {
}
