include($$(GBS_HOME)/common-dll.pri)
TARGET = cwt-debby
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../pfs/include
INCLUDEPATH += ../../../cwt-sys/include
#INCLUDEPATH += ../../../cwt-io/include
#INCLUDEPATH += ../../../cwt-json/include
#INCLUDEPATH += ../../../cwt-uri/include

SOURCES += ../../src/*.cpp

unix {
#    QMAKE_CXXFLAGS += -pg
    LIBS += -lpfs
    LIBS += -lcwt-sys
#    LIBS += -lcwt-json
}

win32 {
    LIBS += cwt.lib
}
