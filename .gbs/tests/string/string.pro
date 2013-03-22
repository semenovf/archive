include($$(GBS_HOME)/common-app.pri)

TARGET = test_string
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

unix {
    SOURCES += ../../../tests/string.cpp
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\string.cpp
    LIBS += cwt.lib
}
