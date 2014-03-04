include($$(GBS_HOME)/qmake/common-dll.pri)

TARGET = cwt-sys

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../pfs/include

# PCRE configuration
DEFINES += HAVE_CONFIG_H
DEFINES += PCRE_EXP_DECL=extern
DEFINES += PCRE_EXP_DEFN=
DEFINES += PCRE_EXP_DATA_DEFN=

unix {
    HEADERS += ../../pcre8/*.h
    SOURCES += ../../src/*.cpp
    SOURCES += ../../pcre8/*.c
    SOURCES += ../../src/unix/*.cpp
    SOURCES += ../../src/x11/*.cpp
    LIBS += -lpfs
    LIBS += -ldl
    LIBS += -lpthread
    LIBS += -lX11
}

win32 {
    HEADERS += ..\\..\\pcre8\\*.h
    SOURCES += ..\\..\\src\\*.cpp
    SOURCES += ..\\..\\pcre8\\*.c
    SOURCES += ..\\..\\src\\win32\\*.cpp
    LIBS += pfs.lib
}
