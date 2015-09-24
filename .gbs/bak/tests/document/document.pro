include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_dom_document

INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../include

SOURCES += ../../../tests/document.cpp

unix {
    LIBS += -lpfs
    LIBS += -lcwt-dom
}

win32 { LIBS += pfs-dom.lib }
