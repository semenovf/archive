CWT_CONFIG=debug
include($$(GBS_HOME)/common-app.pri)
TARGET = test_abnf

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include

SOURCES += ../../../tests/test_abnf.cpp
HEADERS += ../../../include/cwt/abnf.hpp

unix {
    LIBS += -lcwt
    LIBS += -lcwt-abnf
}

win32 {
    LIBS += cwt.lib
    LIBS += cwt-abnf.lib
}
