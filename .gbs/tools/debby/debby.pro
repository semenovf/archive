include($$(GBS_HOME)/qmake/common-app.pri)

TARGET = debby

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tools/debby/*.cpp

unix {
    LIBS += -lcwt
    LIBS += -lcwt-io
    LIBS += -lcwt-json
    LIBS += -lcwt-fs
    LIBS += -lcwt-app
    LIBS += -lcwt-debby
    LIBS += -lcwt-uri
}

win32 {
}
