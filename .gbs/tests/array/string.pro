include($$(GBS_HOME)/common-app.pri)

TARGET = test_array
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

unix {
    SOURCES += ../../../tests/array.cpp
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\array.cpp
    LIBS += cwt.lib
}
