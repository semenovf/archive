include($$(GBS_HOME)/common-app.pri)

TARGET = test_string
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

SOURCES += ../../../tests/string.cpp

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
