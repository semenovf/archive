include($$(GBS_HOME)/common-test.pri)

TARGET = test_vector

INCLUDEPATH += ../../../include
SOURCES += ../../../tests/vector.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }
