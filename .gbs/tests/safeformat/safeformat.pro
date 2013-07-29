include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_safeformat
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

SOURCES += ../../../tests/safeformat.cpp

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
