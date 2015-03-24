include(common.pri)

DESTDIR=$$CWT_DESTDIR_TEST
INCLUDEPATH=$$(GBS_PDIR)/pfs-test/include

CONFIG += qt
QT += core

INCLUDEPATH += $$(QT_HOME)/include
DEFINES += PFS_HAVE_QT

unix {
    LIBS += -lpfs-test
    LIBS += -L$$(QT_HOME)/lib
}

win32 {
    LIBS += pfs-test.lib
}
