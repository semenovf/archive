CWT_CONFIG=debug
include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_settings
DEFINES += __CWT_TEST__

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
SOURCES += ../../../tests/settings.cpp
HEADERS += ../../../../cwt-common/include/*.h
HEADERS += ../../../cwt-common/include/*.h
HEADERS += ../../../cwt-common/include/*.hpp

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
