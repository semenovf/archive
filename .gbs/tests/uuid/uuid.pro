CWT_CONFIG=debug

include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_uuid

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include

SOURCES += ../../../tests/uuid.cpp

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
