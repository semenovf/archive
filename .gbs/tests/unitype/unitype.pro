include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_unitype
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

unix {
    SOURCES += ../../../tests/unitype.cpp
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\unitype.cpp
    LIBS += cwt.lib
}
