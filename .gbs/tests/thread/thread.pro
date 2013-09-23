include($$(GBS_HOME)/common-app.pri)

TARGET = test_thread
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

SOURCES += ../../../tests/thread.cpp

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
