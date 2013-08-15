include($$(GBS_HOME)/common-app.pri)

TARGET = test_vector
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

SOURCES += ../../../tests/vector.cpp

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
