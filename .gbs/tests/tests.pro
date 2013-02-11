CWT_CONFIG=debug
include($$(GBS_HOME)/common-app-qt.pri)
TARGET = test_abnf
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include
INCLUDEPATH += ../../../cwt-core/include

unix {
    SOURCES += ../../tests/test_abnf.cpp
    LIBS += -lcwt
    LIBS += -lcwt-abnf
}

win32 {
    SOURCES += ..\\..\\tests\\test_abnf.cpp
    LIBS += cwt.lib
    LIBS += cwt-abnf.lib
}
