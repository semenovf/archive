include($$(GBS_HOME)/common-app.pri)

TARGET = test_vector
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

unix {
    SOURCES += ../../../tests/vector.cpp
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\vector.cpp
    LIBS += cwt.lib
}
