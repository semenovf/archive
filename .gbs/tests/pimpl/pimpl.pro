include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_pimpl

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/pimpl.cpp
SOURCES += ../../../tests/pimpl_p.cpp

unix {
    LIBS += -lpfs
}

win32 {
    LIBS += pfs.lib
}
