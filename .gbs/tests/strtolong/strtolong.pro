include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_strtolong

INCLUDEPATH += ../../../include

SOURCES += ../../../tests/strtolong.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }
