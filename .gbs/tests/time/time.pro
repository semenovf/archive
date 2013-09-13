include($$(GBS_HOME)/common-app.pri)

TARGET = test_time
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

SOURCES += ../../../tests/time.cpp

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
