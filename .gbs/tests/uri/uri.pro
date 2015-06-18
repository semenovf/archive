include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_uri

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/uri.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
}

win32 {
    LIBS += pfs.lib
    LIBS += pfs-sys.lib
}
