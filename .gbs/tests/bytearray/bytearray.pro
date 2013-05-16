include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_bytearray
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

unix {
    SOURCES += ../../../tests/bytearray.cpp
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\bytearray.cpp
    LIBS += cwt.lib
}
