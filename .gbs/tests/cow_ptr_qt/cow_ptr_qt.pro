include($$(GBS_HOME)/qmake/common-app-qt.pri)

TARGET = test_cow_ptr_qt

INCLUDEPATH += ../../../include
SOURCES += ../../../tests/cow_ptr_qt.cpp

#unix  { LIBS += -lpfs }
#win32 { LIBS += pfs.lib }
