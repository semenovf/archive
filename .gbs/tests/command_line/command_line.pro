include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_command_line

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/command_line.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
    LIBS += -lpfs-app
    LIBS += -lpfs-settings
}

win32 {
    LIBS += pfs.lib
    LIBS += pfs-sys.lib
    LIBS += pfs-app.lib
    LIBS += pfs-settings.lib
}
