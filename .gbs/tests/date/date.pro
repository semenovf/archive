include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_date

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/date.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }
