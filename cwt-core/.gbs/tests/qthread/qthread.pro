include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_qthread
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

SOURCES += ../../../tests/qthread.cpp

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
