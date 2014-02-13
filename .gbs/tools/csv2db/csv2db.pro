include($$(GBS_HOME)/qmake/common-app.pri)

TARGET = csv2db

INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include
INCLUDEPATH += ../../../../cwt-core/include
INCLUDEPATH += ../../../../cwt-io/include
INCLUDEPATH += ../../../../cwt-json/include
INCLUDEPATH += ../../../../cwt-csv/include
INCLUDEPATH += ../../../../cwt-app/include

SOURCES += ../../../tools/csv2db/*.cpp

unix {
    LIBS += -lcwt-debby
    LIBS += -lcwt-csv
    LIBS += -lcwt-app
}

win32 {
}
