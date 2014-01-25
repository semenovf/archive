include($$(GBS_HOME)/common-test.pri)

TARGET = test_dom_utils

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include

SOURCES += ../../../tests/utils.cpp

unix {
    LIBS += -lpfs
    LIBS += -lcwt-dom
}

win32 {
 LIBS += cwt-dom.lib
 }
