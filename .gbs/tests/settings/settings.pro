CWT_CONFIG=debug
include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_settings
DEFINES += __CWT_TEST__

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include
INCLUDEPATH += ../../../../cwt-fs/include
INCLUDEPATH += ../../../../cwt-app/include

SOURCES     += ../../../tests/settings.cpp

unix {
    LIBS += -lcwt
    LIBS += -lcwt-json
    LIBS += -lcwt-logger
    LIBS += -lcwt-io
    LIBS += -lcwt-fs
    LIBS += -lcwt-app
}

win32 {
#    LIBS += cwt.lib
}
