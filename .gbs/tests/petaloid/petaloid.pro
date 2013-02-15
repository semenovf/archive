CWT_CONFIG=debug

include($$(GBS_HOME)/common-petaloid.pri)

TARGET = petaloid-tmpl
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

unix {
    SOURCES += ../../../tests/petaloid_tmpl.cpp
#    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\petaloid_tmpl.cpp
}
