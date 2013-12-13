include($$(GBS_HOME)/common-test.pri)

TARGET = test_dom_domimpl

INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../include

SOURCES += ../../../tests/domimpl.cpp

unix {
    LIBS += -lcwt-dom
}

win32 { LIBS += cwt-dom.lib }
