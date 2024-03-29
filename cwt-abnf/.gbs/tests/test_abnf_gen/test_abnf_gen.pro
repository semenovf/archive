include($$(GBS_HOME)/qmake/common-app.pri)
TARGET = test_abnf_gen

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include

SOURCES += ../../../tests/test_abnf_gen.cpp
HEADERS += ../../../include/cwt/abnf.hpp


unix {
    LIBS += -lcwt
    LIBS += -lcwt-abnf
}

win32 {
    LIBS += cwt.lib
    LIBS += cwt-abnf.lib
}
