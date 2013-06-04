CWT_CONFIG=debug
include($$(GBS_HOME)/common-app-qt.pri)
TARGET = test_json

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
SOURCES += ../../../tests/json.cpp
HEADERS += ../../../../cwt-common/include/*.h
HEADERS += ../../../cwt-common/include/*.h
HEADERS += ../../../cwt-common/include/*.hpp

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
