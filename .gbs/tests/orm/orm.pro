include($$(GBS_HOME)/qmake/common-test.pri)

TARGET = test_debby_orm

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/orm.cpp

unix {
    LIBS += -lpfs
    LIBS += -lcwt-sys
    LIBS += -lcwt-debby
}

win32 {
}
