include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_async
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

SOURCES += ../../../tests/async_qt.cpp

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
