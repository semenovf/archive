include($$(GBS_HOME)/common-test.pri)

TARGET = test_time

INCLUDEPATH += ../../../include

SOURCES += ../../../tests/time.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }