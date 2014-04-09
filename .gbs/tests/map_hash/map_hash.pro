include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_map_hash

INCLUDEPATH += ../../../include
SOURCES += ../../../tests/map_hash.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }
