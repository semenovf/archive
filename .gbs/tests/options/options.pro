include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_options
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include

SOURCES += ../../../tests/options.cpp

unix {
    LIBS += -lcwt
    LIBS += -lcwt-json
    LIBS += -lcwt-app
}

win32 {
    LIBS += cwt.lib
}
