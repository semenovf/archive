INCLUDEPATH += ../../include
INCLUDEPATH += ../../src
INCLUDEPATH += ../../../pfs/include
INCLUDEPATH += ../../../pfs-sys/include
INCLUDEPATH += ../../../pfs-app/include

DEFINES += _SIMULATOR

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
}

win32 {
}
