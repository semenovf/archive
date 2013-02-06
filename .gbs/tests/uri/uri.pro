include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_uri
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

unix {
    SOURCES += ../../../tests/uri.cpp
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\uri.cpp
    LIBS += cwt.lib
}
