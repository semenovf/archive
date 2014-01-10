include($$(GBS_HOME)/common-test.pri)

TARGET = test_dom_document

INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../include

SOURCES += ../../../tests/document.cpp

unix {
    LIBS += -lcwt-dom
}

win32 { LIBS += cwt-dom.lib }
