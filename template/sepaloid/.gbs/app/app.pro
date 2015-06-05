include($$(GBS_HOME)/qmake/common-app.pri)
include(../common.pri)

TARGET = $$GBS_TARGET_NAME-app

SOURCES += ../../src/*.cpp

unix {
    LIBS += -lpfs-settings
    LIBS += -lpfs-app
}

win32 {
    LIBS += pfs-settings.lib
    LIBS += pfs-app.lib
}

include($$(GBS_HOME)/qmake/deploy.pri)
