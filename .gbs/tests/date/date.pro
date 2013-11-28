include($$(GBS_HOME)/common-test.pri)

TARGET = test_date

INCLUDEPATH += ../../../include

SOURCES += ../../../tests/date.cpp

unix  { LIBS += -lpfs }
win32 { LIBS += pfs.lib }
