CWT_CONFIG=debug
include($$(GBS_HOME)/common-app-qt.pri)
TARGET = test_sepaloid

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include
INCLUDEPATH += ../../../../cwt-app/include

SOURCES += ../../../tests/sepaloid.cpp

unix {
    LIBS += -lcwt
    LIBS += -lcwt-json
    LIBS += -lcwt-app
}

win32 {
}
