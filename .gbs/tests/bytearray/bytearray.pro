include($$(GBS_HOME)/common-test.pri)

TARGET = test_bytearray

INCLUDEPATH += ../../../include
SOURCES += ../../../tests/bytearray.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }
