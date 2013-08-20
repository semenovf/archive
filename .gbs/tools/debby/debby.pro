CWT_CONFIG=debug

include($$(GBS_HOME)/common-app.pri)

TARGET = debby

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include
INCLUDEPATH += ../../../../cwt-logger/include
INCLUDEPATH += ../../../../cwt-io/include
INCLUDEPATH += ../../../../cwt-json/include
INCLUDEPATH += ../../../../cwt-app/include

SOURCES += ../../../tools/debby/*.cpp

unix {
    LIBS += -lcwt
    LIBS += -lcwt-io
    LIBS += -lcwt-json
    LIBS += -lcwt-logger
    LIBS += -lcwt-fs
    LIBS += -lcwt-app
    LIBS += -lcwt-debby
    LIBS += -lcwt-uri
}

win32 {
}
