win32: include(/qmake/common-dll.pri)
unix: include($$(HOME)/ws/qmake/common-dll.pri)


TARGET = cwt-pcre

INCLUDEPATH += ../../include
INCLUDEPATH += ../../include/cwt/pcre

# comment this in production mode
DEFINES += CWT_USE_TESTS


unix {
    SOURCES += ../../src/pcre/*.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\src\\pcre\\*.c

#    LIBS += Advapi32.lib
    LIBS += cwt.lib
}
