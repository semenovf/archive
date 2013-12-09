include($$(GBS_HOME)/common-test.pri)

TARGET = test_bitarray

INCLUDEPATH += ../../../include
SOURCES += ../../../tests/bitarray.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }
