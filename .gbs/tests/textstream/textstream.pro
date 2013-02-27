include($$(GBS_HOME)/common-app-qt.pri)

TARGET = test_textstream
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

unix {
    SOURCES += ../../../tests/textstream.cpp
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\textstream.cpp
    LIBS += cwt.lib
}
