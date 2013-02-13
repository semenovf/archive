CWT_CONFIG=debug

include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_sepaloid

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include

unix {
    SOURCES += ../../../tests/sepaloid.cpp
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\sepaloid.cpp
    LIBS += cwt.lib
}
