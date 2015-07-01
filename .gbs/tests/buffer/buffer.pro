include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test-pfs-io-buffer

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/buffer.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
    LIBS += -lpfs-io
}

win32 {
    LIBS += pfs.lib
    LIBS += pfs-sys.lib
    LIBS += pfs-io.lib
}
