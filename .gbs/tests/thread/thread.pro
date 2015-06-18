include($$(GBS_HOME)/qmake/common-test-qt.pri)

TARGET = test_pfs_thread

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/thread.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
}

win32 {
    LIBS += pfs.lib
    LIBS += pfs-sys.lib
}
