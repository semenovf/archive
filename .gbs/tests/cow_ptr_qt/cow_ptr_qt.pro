include($$(GBS_HOME)/qmake/common-app-qt.pri)

TARGET = test_pfs_cow_ptr_qt

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/cow_ptr_qt.cpp

#unix  { LIBS += -lpfs }
#win32 { LIBS += pfs.lib }
