CWT_CONFIG=debug
include($$(GBS_HOME)/common-dll.pri)
TARGET = cwt-debby
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include
INCLUDEPATH += ../../../cwt-core/include
INCLUDEPATH += ../../../cwt-io/include
INCLUDEPATH += ../../../cwt-json/include
INCLUDEPATH += ../../../cwt-uri/include

SOURCES += ../../src/*.cpp

unix {
    LIBS += -lcwt
    LIBS += -lcwt-io
    LIBS += -lcwt-json
    LIBS += -lcwt-fs
    LIBS += -lcwt-uri
}

win32 {
    LIBS += cwt.lib
}
