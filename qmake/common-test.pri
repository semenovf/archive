include(common-app.pri)

INCLUDEPATH=$$(GBS_PDIR)/cwt-test/include

unix {
    LIBS += -lcwt-test
}

win32 {
    LIBS += cwt-test.lib
}
