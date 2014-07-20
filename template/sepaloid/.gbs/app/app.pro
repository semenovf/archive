include($$(GBS_HOME)/qmake/common-app.pri)
include(../common.pri)

TARGET = $$GBS_TARGET_NAME

HEADERS += ../../include/*.hpp
SOURCES += ../../src/*.cpp

unix {
    LIBS += -lcwt-app
}

win32 {
}
