CWT_CONFIG=debug

include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_uuid

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include

unix {
    SOURCES += ../../../tests/uuid.cpp
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\uuid.cpp
    LIBS += cwt.lib
}
