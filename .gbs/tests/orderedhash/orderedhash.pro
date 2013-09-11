CWT_CONFIG=debug

include($$(GBS_HOME)/common-app.pri)

TARGET = test_orderedhash

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

SOURCES += ../../../tests/test_orderedhash.cpp

unix {
    LIBS += -lcwt
}

win32 {
}
