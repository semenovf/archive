CWT_CONFIG=debug
include($$(GBS_HOME)/common-app-qt.pri)
TARGET = abnf2fsm
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include
INCLUDEPATH += ../../../cwt-core/include

unix {
    SOURCES += ../../tools/abnf2fsm.cpp
    LIBS += -lcwt
    LIBS += -lcwt-abnf
}

win32 {
    SOURCES += ..\\..\\tools\\abnf2fsm.cpp
    LIBS += cwt.lib
    LIBS += cwt-abnf.lib
}
