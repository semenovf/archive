include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_fsm
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

unix {
    SOURCES += ../../../tests/fsm.cpp
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\fsm.cpp
    LIBS += cwt.lib
}
