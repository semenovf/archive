CWT_CONFIG=debug
include($$(GBS_HOME)/common-app.pri)
TARGET = abnf2fsm
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include
INCLUDEPATH += ../../../cwt-core/include

SOURCES += ../../tools/abnf2fsm.cpp

unix {
    LIBS += -lcwt
    LIBS += -lcwt-abnf
}

win32 {
    LIBS += cwt.lib
    LIBS += cwt-abnf.lib
}
