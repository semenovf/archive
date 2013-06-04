include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_unitype
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
SOURCES += ../../../tests/unitype.cpp
HEADERS += ../../../../cwt-common/include/*.h
HEADERS += ../../../cwt-common/include/*.h
HEADERS += ../../../cwt-common/include/*.hpp

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
