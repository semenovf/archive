include($$(GBS_HOME)/common-app.pri)

TARGET = example_app

INCLUDEPATH += ../../../include

unix {
    SOURCES += ../../../examples/app.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\examples\\app.c
    LIBS += cwt.lib
}
