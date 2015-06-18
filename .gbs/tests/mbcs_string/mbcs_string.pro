include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_mbcs_string

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/mbcs_string.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }
