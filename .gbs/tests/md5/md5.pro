CWT_CONFIG=debug

include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_md5

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include

SOURCES += ../../../tests/md5.c

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
