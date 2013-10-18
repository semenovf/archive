include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_collection
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

SOURCES += ../../../tests/collection.cpp

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
