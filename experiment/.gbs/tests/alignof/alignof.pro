include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_alignof

INCLUDEPATH += ../../../include
SOURCES += ../../../tests/alignof.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }
