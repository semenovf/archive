include($$(GBS_HOME)/common-test.pri)

TARGET = test_shared_ptr

INCLUDEPATH += ../../../include
SOURCES += ../../../tests/shared_ptr.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }
