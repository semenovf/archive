CWT_CONFIG=debug
include($$(GBS_HOME)/common-dll.pri)
TARGET = cwt-app
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include
INCLUDEPATH += ../../../cwt-core/include
INCLUDEPATH += ../../../cwt-logger/include
INCLUDEPATH += ../../../cwt-io/include
INCLUDEPATH += ../../../cwt-fs/include
INCLUDEPATH += ../../../cwt-json/include

HEADERS += ../../include/cwt/*.hpp
SOURCES += ../../src/*.cpp

unix {
    LIBS += -lcwt
    LIBS += -lcwt-io
    LIBS += -lcwt-json
    LIBS += -lcwt-logger
}

win32 {
#   LIBS += lmylib.lib
}
