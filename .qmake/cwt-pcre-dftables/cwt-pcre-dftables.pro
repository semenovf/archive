win32: include(/qmake/common-app.pri)
unix: include($$(HOME)/ws/qmake/common-app.pri)


TARGET = cwt-pcre-dftables

INCLUDEPATH += ../../include
INCLUDEPATH += ../../include/cwt/pcre

# comment this in production mode
DEFINES += CWT_USE_TESTS


unix {
    SOURCES += ../../src/pcre/dftables/*.c
#    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\src\\pcre\\dftables\\*.c

#    LIBS += Advapi32.lib
#    LIBS += cwt.lib
}
