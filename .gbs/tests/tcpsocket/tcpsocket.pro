CWT_CONFIG=debug

include($$(GBS_HOME)/common-app.pri)

TARGET = test_tcpsocket

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include
INCLUDEPATH += ../../../../cwt-io/include

SOURCES += ../../../tests/tcpsocket.cpp
HEADERS += ../../../include/cwt/io/*.hpp
HEADERS += ../../../include/cwt/net/*.hpp

unix {
    LIBS += -lcwt
    LIBS += -lcwt-io
}

win32 {
}
