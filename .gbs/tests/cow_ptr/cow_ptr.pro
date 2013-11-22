include($$(GBS_HOME)/common-test.pri)

TARGET = test_cow_ptr

INCLUDEPATH += ../../../include
SOURCES += ../../../tests/cow_ptr.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }
