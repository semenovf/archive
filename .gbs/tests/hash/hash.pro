include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_hash
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

unix {
    SOURCES += ../../../tests/hash.cpp
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\hash.cpp
    LIBS += cwt.lib
}
