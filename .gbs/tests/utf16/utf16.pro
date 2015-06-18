include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_utf16

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/utf16.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }
