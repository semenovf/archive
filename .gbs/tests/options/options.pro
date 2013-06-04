include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_options
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
SOURCES += ../../../tests/options.cpp
HEADERS += ../../../../cwt-common/include/*.h
HEADERS += ../../../cwt-common/include/*.h
HEADERS += ../../../cwt-common/include/*.hpp

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
