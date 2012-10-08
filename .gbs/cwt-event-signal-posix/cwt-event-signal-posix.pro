include($$(GBS_HOME)/common-dll.pri)

TARGET = cwt-event-signal-posix

INCLUDEPATH += ../../include

unix {
    SOURCES += ../../src/event/posix/esignal-posix.c
    LIBS += -lcwt
}

win32 {
}
