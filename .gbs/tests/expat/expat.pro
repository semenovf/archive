include($$(GBS_HOME)/common-test.pri)

TARGET = test_expat

INCLUDEPATH += ../../../include

SOURCES += ../../../expat2/tests/*.c

unix {
    # disable treat of 'unused parameter' as error (restore it's warning status)
    QMAKE_CFLAGS += -Wno-error=unused-parameter

    # suppress output of 'unused parameter' warning
    QMAKE_CFLAGS += -Wno-unused-parameter

    QMAKE_CFLAGS += -Wno-error=missing-field-initializers
    QMAKE_CFLAGS += -Wno-missing-field-initializers

    LIBS += -lcwt-xml
}

win32 { LIBS += cwt-xml.lib }
