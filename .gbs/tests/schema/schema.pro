include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_debby_schema

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../../cwt-sys/include

SOURCES += ../../../tests/schema.cpp

unix {
    LIBS += -lpfs
    LIBS += -lcwt-sys
    LIBS += -lcwt-debby
}

win32 {
}
