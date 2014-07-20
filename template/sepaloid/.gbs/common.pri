GBS_TARGET_NAME=hg-booker

INCLUDEPATH += ../../include
INCLUDEPATH += ../../src
INCLUDEPATH += ../../../pfs/include
INCLUDEPATH += ../../../cwt-sys/include
INCLUDEPATH += ../../../cwt-app/include

DEFINES += _SIMULATOR

unix {
    LIBS += -lpfs
    LIBS += -lcwt-sys
}

win32 {
}
