include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_pfs_debby_schema

INCLUDEPATH += ../../../include

SOURCES += ../../../tests/schema.cpp

unix {
    LIBS += -lpfs
    LIBS += -lpfs-sys
    LIBS += -lpfs-debby
}

win32 {
}
