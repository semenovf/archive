include($$(GBS_HOME)/qmake/common-app.pri)

TARGET = test_debby_hiberlite

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../../cwt-sys/include
#INCLUDEPATH += ../../../../cwt-io/include
#INCLUDEPATH += ../../../../cwt-csv/include

SOURCES += ../../../tests/hiberlite.cpp

unix {
    LIBS += -lpfs
    LIBS += -lcwt-sys
    LIBS += -lcwt-debby
#    LIBS += -lcwt-csv
}

win32 {
}
