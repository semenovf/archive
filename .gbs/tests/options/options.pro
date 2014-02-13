include($$(GBS_HOME)/qmake/common-app-qt.pri)

TARGET = test_options
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../../cwt-sys/include

SOURCES += ../../../tests/options.cpp

unix {
    LIBS += -lpfs
    LIBS += -lcwt-sys
#    LIBS += -lcwt-json
#    LIBS += -lcwt-io
    LIBS += -lcwt-app
}

win32 {
    LIBS += cwt.lib
}
