include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_vector

INCLUDEPATH += ../../../include

SOURCES += ../../../tests/vector.cpp

#unix  { LIBS += -lpfs }
#win32 { LIBS += pfs.lib }
