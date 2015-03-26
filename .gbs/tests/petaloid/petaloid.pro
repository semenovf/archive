include($$(GBS_HOME)/qmake/common-petaloid.pri)

TARGET = petaloid-tmpl
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../pfs/include
INCLUDEPATH += ../../../../pfs-sys/include
INCLUDEPATH += ../../../../pfs-app/include
SOURCES += ../../../tests/petaloid_tmpl.cpp

unix {
}

win32 {
}
