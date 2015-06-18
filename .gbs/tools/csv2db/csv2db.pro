include($$(GBS_HOME)/qmake/common-app.pri)

TARGET = csv2db

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tools/csv2db/*.cpp

unix {
    LIBS += -lcwt-debby
    LIBS += -lcwt-csv
    LIBS += -lcwt-app
}

win32 {
}
