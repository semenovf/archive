include($$(GBS_HOME)/qmake/common-app.pri)
include(../common.pri)

TARGET = $$GBS_TARGET_NAME

HEADERS += ../../include/*.hpp
SOURCES += ../../src/*.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
    LIBS += -lpfs-settings
    LIBS += -lpfs-app
}

win32 {
}
