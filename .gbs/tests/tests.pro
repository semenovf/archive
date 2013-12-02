CWT_CONFIG=debug

include($$(GBS_HOME)/common-app.pri)

TARGET = test_dom

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../pfs/include
INCLUDEPATH += ../../../cwt-test/include

SOURCES += ../../tests/test.cpp

unix {
    LIBS += -lcwt-test
    LIBS += -lcwt-dom
}

win32 {
    LIBS += cwt-test.lib
    LIBS += cwt-dom.lib
}
