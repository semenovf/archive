include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_byte_string

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/byte_string.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }
