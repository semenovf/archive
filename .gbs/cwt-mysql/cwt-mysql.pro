include($$(GBS_HOME)/common-dll.pri)


TARGET = cwt-mysql

INCLUDEPATH += ../../include
INCLUDEPATH += ../../include/mysql

# comment this in production mode
DEFINES += CWT_USE_TESTS


unix {
    SOURCES += ../../src/dbi/driver/mysql/*.c
    LIBS += -lcwt
    LIBS += -lmysql
}

win32 {
    SOURCES += ..\\..\\src\\dbi\\driver\\mysql\\*.c

#    LIBS += Advapi32.lib
    LIBS += cwt.lib
    LIBS += libmysql.lib
}
