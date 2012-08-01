win32: include(/qmake/common-dll.pri)
unix: include($$(HOME)/ws/qmake/common-dll.pri)


TARGET = cwt-mysql

INCLUDEPATH += ../../include
INCLUDEPATH += $$CWT_ROOT_DIR/include/mysql

# comment this in production mode
DEFINES += CWT_USE_TESTS


unix {
    SOURCES += ../../src/dbi/driver/mysql/mysql.c
    LIBS += -lcwt
    LIBS += -lmysql
}

win32 {
    SOURCES += ..\\..\\src\\dbi\\driver\\mysql\\mysql.c

#    LIBS += Advapi32.lib
    LIBS += cwt.lib
    LIBS += libmysql.lib
}
