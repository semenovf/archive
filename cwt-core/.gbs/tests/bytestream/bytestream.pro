include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_bytestream
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

unix {
    SOURCES += ../../../tests/bytestream.cpp
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\bytestream.cpp
    LIBS += cwt.lib
}
