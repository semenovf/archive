include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_real_cast

INCLUDEPATH += ../../../include

SOURCES += ../../../tests/real_cast.cpp

#unix  { LIBS += -lpfs }
#win32 { LIBS += pfs.lib }
