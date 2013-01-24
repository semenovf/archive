CWT_CONFIG=debug
include($$(GBS_HOME)/common-app.pri)
TARGET =
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include

unix {
    HEADERS += ../../include/hg/*.h
    SOURCES += ../../src/*.c

#    LIBS += -lmysql
}

win32 {
    HEADERS += ..\\..\\include\\hg\\*.h
    SOURCES += ..\\..\\src\\*.c
#    LIBS += cwt.lib
}
