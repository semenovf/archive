include($$(GBS_HOME)/common-dll.pri)

TARGET = cwt-csv

INCLUDEPATH += ../../include
INCLUDEPATH += ../../include/cwt/csv

# comment this in production mode
DEFINES += CWT_USE_TESTS


unix {
    SOURCES += ../../src/csv/*.c
    LIBS += -lcwt
}

win32 {
    SOURCES += ..\\..\\src\\csv\\*.c

#    LIBS += Advapi32.lib
    LIBS += cwt.lib
}
