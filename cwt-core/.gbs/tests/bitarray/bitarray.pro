include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_bitarray
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

SOURCES += ../../../tests/bitarray.cpp

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
