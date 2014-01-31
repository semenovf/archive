include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_settings
DEFINES += __CWT_TEST__

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-sys/include
INCLUDEPATH += ../../../../cwt-app/include

SOURCES     += ../../../tests/settings.cpp

unix {
    LIBS += -lpfs
    LIBS += -lcwt-sys
#    LIBS += -lcwt-json
    LIBS += -lcwt-app
}

win32 {
#    LIBS += cwt.lib
}
