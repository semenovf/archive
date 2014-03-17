include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_pimpl

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
#INCLUDEPATH += ../../../../cwt-test/include

SOURCES += ../../../tests/pimpl.cpp
SOURCES += ../../../tests/pimpl_p.cpp

unix {
    LIBS += -lpfs
}

win32 {
    LIBS += pfs.lib
}
