include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_unitype
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
SOURCES += ../../../tests/unitype.cpp

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
