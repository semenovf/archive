include($$(GBS_HOME)/qmake/common-app.pri)
TARGET = abnf2fsm
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include
INCLUDEPATH += ../../../cwt-core/include

SOURCES += ../../tools/abnf2fsm.cpp
SOURCES += ../../tools/abnf_rulelist.cpp
HEADERS += ../../include/cwt/abnf.hpp


unix {
    LIBS += -lcwt
    LIBS += -lcwt-abnf
}

win32 {
    LIBS += cwt.lib
    LIBS += cwt-abnf.lib
}
