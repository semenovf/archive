include($$(GBS_HOME)/common-app.pri)

TARGET = test_shared_ptr
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

SOURCES += ../../../tests/shared_ptr.cpp

unix {
    LIBS += -lcwt
}

win32 {
    LIBS += cwt.lib
}
