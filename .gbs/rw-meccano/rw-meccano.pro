include($$(GBS_HOME)/qmake/common-app-qt-gui.pri)
TARGET = rw-meccano
QT += svg

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../pfs/include
INCLUDEPATH += ../../../cwt-sys/include

HEADERS   += ../../src/*.hpp
SOURCES   += ../../src/*.cpp

#RESOURCES += ../../rc/abtcws.qrc
#FORMS = ../../src/ui/qt/ui/*.ui

unix {
    system(rcc -binary ../../rc/rw-meccano.qrc -o $$OBJECTS_DIR/rw-meccano.rcc)
    system(mv $$OBJECTS_DIR/rw-meccano.rcc $$CWT_DESTDIR_LIB/)

    LIBS += -lpfs
    LIBS += -lcwt-sys
    LIBS += -lcwt-dom
    LIBS += -lcwt-xml
}

win32 {
    LIBS += pfs.lib
    LIBS += cwt-sys.lib
}
