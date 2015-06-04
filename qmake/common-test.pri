include(common.pri)

CWT_CONFIG += gbs_test

DESTDIR=$$CWT_DESTDIR_TEST

unix {
    LIBS += -lpfs-test
}

win32 {
    LIBS += pfs-test.lib
}
