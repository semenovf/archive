include($$(GBS_HOME)/common-app.pri)

TARGET = test_shared_ptr
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

unix {
    SOURCES += ../../../tests/shared_ptr.cpp
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\..\\tests\\shared_ptr.cpp
    LIBS += cwt.lib
}
