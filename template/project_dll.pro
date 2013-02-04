CWT_CONFIG=debug
include($$(GBS_HOME)/common-dll.pri)
TARGET =
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include

unix {
    HEADERS += ../../include/@PRJ@/*.h
    SOURCES += ../../src/*.c
#   LIBS += -lmylib
}

win32 {
    HEADERS += ..\\..\\include\\@PRG@\\*.h
    SOURCES += ..\\..\\src\\*.c
#   LIBS += lylib.lib
}
