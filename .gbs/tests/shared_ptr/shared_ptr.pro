include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_shared_ptr

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/shared_ptr.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }
