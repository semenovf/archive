include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_utf8

INCLUDEPATH += ../../../include
SOURCES += ../../../tests/utf8.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }