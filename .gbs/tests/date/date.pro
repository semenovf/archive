include($$(GBS_HOME)/common-app.pri)

TARGET = test_date
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

SOURCES += ../../../tests/date.cpp

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
