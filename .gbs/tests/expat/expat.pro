include($$(GBS_HOME)/common-test.pri)

TARGET = test_expat

INCLUDEPATH += ../../../include

SOURCES += ../../../expat2/tests/*.c

unix {
    QMAKE_CFLAGS += -Wno-error=unused-parameter
    LIBS += -lcwt-xml
}

win32 { LIBS += cwt-xml.lib }
