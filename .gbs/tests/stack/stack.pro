include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_stack

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/stack.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }
