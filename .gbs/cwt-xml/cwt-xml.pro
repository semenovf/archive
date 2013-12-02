CWT_CONFIG=debug
include($$(GBS_HOME)/common-dll.pri)
TARGET = cwt-xml
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../pfs/include

SOURCES += ../../expat2/lib/xmlparse.c
SOURCES += ../../expat2/lib/xmlrole.c
SOURCES += ../../expat2/lib/xmltok.c
SOURCES += ../../expat2/lib/xmltok_ns.c
#SOURCES += ../../src/*.cpp

DEFINES += XML_DTD=1
DEFINES += XML_NS=1
DEFINES += XML_CONTEXT_BYTES=1024
DEFINES += XML_MIN_SIZE

unix {
# TODO need to checking programmaticaly below defines.
    DEFINES += HAVE_MEMMOVE
    DEFINES += HAVE_BCOPY

    QMAKE_CFLAGS += -Wno-error=unused-parameter

    LIBS += -lpfs
}

win32 {
    LIBS += pfs.lib
}
