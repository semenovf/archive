include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_bit_array

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/bit_array.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }
