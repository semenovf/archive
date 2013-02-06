CWT_CONFIG=debug
include($$(GBS_HOME)/common-app.pri)
TARGET =
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include

unix {
    HEADERS += ../../include/@PRG@/*.h
    SOURCES += ../../src/*.c
#   LIBS += -lmylib
}

win32 {
    HEADERS += ..\\..\\include\\@PRG@\\*.h
    SOURCES += ..\\..\\src\\*.c
#   LIBS += lmylib.lib
}
