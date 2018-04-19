include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_dl
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

unix {
    SOURCES += ../../../tests/dl.cpp
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\dl.cpp
    LIBS += cwt.lib
}
