include($$(GBS_HOME)/common-app.pri)

TARGET = test_utf8string
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include

SOURCES += ../../../tests/utf8string.cpp

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
