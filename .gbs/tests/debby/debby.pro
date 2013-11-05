CWT_CONFIG=debug

include($$(GBS_HOME)/common-app.pri)

TARGET = test_debby

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include

SOURCES += ../../../tests/test_debby.cpp

unix {
    LIBS += -lcwt
    LIBS += -lcwt-debby
}

win32 {
}
