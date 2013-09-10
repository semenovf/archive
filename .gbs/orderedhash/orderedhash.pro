CWT_CONFIG=debug

include($$(GBS_HOME)/common-app.pri)

TARGET = test_orderedhash

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../cwt-common/include
INCLUDEPATH += ../../../cwt-core/include

SOURCES += ../../tests/test_orderedhash.cpp

unix {
    LIBS += -lcwt
    LIBS += -lcwt-debby
}

win32 {
}
