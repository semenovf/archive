include($$(GBS_HOME)/qmake/common-petaloid.pri)

TARGET = petaloid-tmpl

INCLUDEPATH = $$prependIncludePath(../../../include)
SOURCES += ../../../tests/petaloid_tmpl.cpp

unix {
}

win32 {
}
