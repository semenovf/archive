include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_bytearray

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

SOURCES += ../../../tests/bytearray.cpp

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
