include($$(GBS_HOME)/common-dll.pri)
TARGET = cwt-app
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../pfs/include
INCLUDEPATH += ../../../cwt-sys/include
#INCLUDEPATH += ../../../cwt-io/include
#INCLUDEPATH += ../../../cwt-json/include

HEADERS += ../../include/cwt/*.hpp
SOURCES += ../../src/*.cpp

unix {
    LIBS += -lpfs
    LIBS += -lcwt-sys
#    LIBS += -lcwt-io
#    LIBS += -lcwt-json
}

win32 {
#   LIBS += lmylib.lib
}
