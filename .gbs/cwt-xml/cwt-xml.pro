CWT_CONFIG=debug
include($$(GBS_HOME)/common-dll.pri)
TARGET = cwt-xml
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../pfs/include

SOURCES += ../../expat2/lib/xmlparse.c
SOURCES += ../../expat2/lib/xmlrole.c
SOURCES += ../../expat2/lib/xmltok.c
SOURCES += ../../expat2/lib/xmltok_ns.c
SOURCES += ../../src/*.cpp

DEFINES += XML_DTD=1
DEFINES += XML_NS=1
DEFINES += XML_CONTEXT_BYTES=1024

unix {
    INCLUDEPATH += ../../expat2/lib
    DEFINES += HAVE_EXPAT_CONFIG_H
    DEFINES += XML_MIN_SIZE

# TODO need to checking programmaticaly below defines.
    DEFINES += HAVE_MEMMOVE
    DEFINES += HAVE_BCOPY

    # disable treat of 'unused parameter' as error (restore it's warning status)
    QMAKE_CFLAGS += -Wno-error=unused-parameter

    # suppress output of 'unused parameter' warning
    QMAKE_CFLAGS += -Wno-unused-parameter

    QMAKE_CFLAGS += -Wno-error=missing-field-initializers
    QMAKE_CFLAGS += -Wno-missing-field-initializers

    LIBS += -lpfs
}

win32 {
    LIBS += pfs.lib
}
