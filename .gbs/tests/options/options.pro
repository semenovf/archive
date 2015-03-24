include($$(GBS_HOME)/qmake/common-app-qt.pri)

TARGET = test_pfs_options
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../../cwt-sys/include

SOURCES += ../../../tests/options.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
    LIBS += -lpfs-app
}

win32 {
#    LIBS += cwt.lib
}
