CWT_CONFIG=debug

include($$(GBS_HOME)/common-app-qt.pri)

TARGET = example_json_prettyprint
HEADERS += ../../../../cwt-common/include/*.h
HEADERS += ../../../cwt-common/include/*.h
HEADERS += ../../../cwt-common/include/*.hpp

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include

SOURCES += ../../../examples/json_prettyprint.cpp

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
