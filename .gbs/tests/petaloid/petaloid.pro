include($$(GBS_HOME)/qmake/common-petaloid.pri)

TARGET = petaloid-tmpl
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../../cwt-sys/include
INCLUDEPATH += ../../../../cwt-app/include
SOURCES += ../../../tests/petaloid_tmpl.cpp

unix {
}

win32 {
}
