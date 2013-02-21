CWT_CONFIG=debug

include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_md5

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include

unix {
    SOURCES += ../../../tests/md5.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\md5.c
    LIBS += cwt.lib
}
