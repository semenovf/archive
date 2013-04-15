include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_file
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

unix {
    SOURCES += ../../../tests/file.cpp
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\file.cpp
    LIBS += cwt.lib
}
