CWT_CONFIG=debug
include($$(GBS_HOME)/common-app.pri)
TARGET = test_abnf_gen

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include

SOURCES += ../../../tests/test_abnf_gen.cpp

unix {
    LIBS += -lcwt
    LIBS += -lcwt-abnf
}

win32 {
    LIBS += cwt.lib
    LIBS += cwt-abnf.lib
}
