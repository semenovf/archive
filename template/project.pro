CWT_CONFIG=debug
include($$(GBS_HOME)/common-app.pri)
TARGET =
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include
INCLUDEPATH += ../../../cwt-core/include

SOURCES += ../../src/*.cpp

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
