include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_endian

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/endian.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }
