include($$(GBS_HOME)/common-app.pri)

TARGET = test_regexp
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

SOURCES += ../../../tests/regexp.cpp

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
