include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_time

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/time.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }
