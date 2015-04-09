include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_variant

INCLUDEPATH += ../../../include
SOURCES += ../../../tests/variant.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }
