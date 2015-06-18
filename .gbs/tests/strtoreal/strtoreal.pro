include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_strtoreal

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/strtoreal.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }
