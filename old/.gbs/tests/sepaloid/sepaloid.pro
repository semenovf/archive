include($$(GBS_HOME)/qmake/common-test.pri)
TARGET = test_pfs_sepaloid

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/sepaloid.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
    LIBS += -lpfs-app
}

win32 {
}
