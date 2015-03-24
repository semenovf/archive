include(common.pri)

DESTDIR=$$CWT_DESTDIR_TEST
INCLUDEPATH=$$(GBS_PDIR)/pfs-test/include

unix {
    LIBS += -lpfs-test
}

win32 {
    LIBS += pfs-test.lib
}
