include($$(GBS_HOME)/common-test.pri)

TARGET = test_dom_node

INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../include

SOURCES += ../../../tests/node.cpp

unix {
    LIBS += -lcwt-dom
}

win32 { LIBS += cwt-dom.lib }
