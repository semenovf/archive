CWT_CONFIG=debug

include($$(GBS_HOME)/common-app.pri)

TARGET = test_fsm

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include

SOURCES += ../../../tests/fsm.cpp

unix {
    LIBS += -lcwt
}

win32 {
}
