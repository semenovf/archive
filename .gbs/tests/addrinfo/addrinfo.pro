CWT_CONFIG=debug

include($$(GBS_HOME)/common-test.pri)

TARGET = test_io_addrinfo

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../../cwt-sys/include
INCLUDEPATH += ../../../../cwt-io/include

SOURCES += ../../../tests/addrinfo.cpp
HEADERS += ../../../include/cwt/io/*.hpp
HEADERS += ../../../include/cwt/net/*.hpp

unix {
    LIBS += -lcwt-io
}

win32 {
}
