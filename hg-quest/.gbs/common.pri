INCLUDEPATH += ../../include
INCLUDEPATH += ../../../pfs/include
INCLUDEPATH += ../../../cwt-sys/include
INCLUDEPATH += ../../../cwt-app/include
#INCLUDEPATH += ../../../cwt-io/include
#INCLUDEPATH += ../../../cwt-fs/include
#INCLUDEPATH += ../../../cwt-json/include

INCLUDEPATH += ../../include

unix {
    LIBS += -lpfs
    LIBS += -lcwt-sys
    LIBS += -lcwt-app
}

win32 {
}
