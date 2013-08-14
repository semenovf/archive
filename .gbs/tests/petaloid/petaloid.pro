CWT_CONFIG=debug

include($$(GBS_HOME)/common-petaloid.pri)

TARGET = petaloid-tmpl
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include
INCLUDEPATH += ../../../../cwt-app/include
SOURCES += ../../../tests/petaloid_tmpl.cpp

unix {
#    LIBS += -lcwt
#    LIBS += -lcwt-json
#    LIBS += -lcwt-io
#    LIBS += -lcwt-logger
#    LIBS += -lcwt-app
}

win32 {
}
