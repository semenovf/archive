include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_safeformat

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/safeformat.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }
