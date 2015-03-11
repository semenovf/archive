include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_strtointegral

INCLUDEPATH += ../../../include

SOURCES += ../../../tests/strtointegral.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }
