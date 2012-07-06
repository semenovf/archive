include(/qmake/common-dll.pri)

TARGET = cwt

INCLUDEPATH += ../../include
INCLUDEPATH += /include/mysql

# comment this in production mode
DEFINES += CWT_USE_TESTS


unix {
    HEADERS += ../../include/cwt/*.h
    HEADERS += ../../include/cwt/algo/*.h
    HEADERS += ../../include/cwt/algo/*.h
    HEADERS += ../../include/cwt/io/*.h
    HEADERS += ../../include/cwt/dbi/*.h
    HEADERS += ../../include/cwt/stylex/*.h
    HEADERS += ../../include/cwt/net/*.h

    SOURCES += ../../*.c
    SOURCES += ../../algo/*.c
    SOURCES += ../../event/*.c
    SOURCES += ../../io/*.c
    SOURCES += ../../stylex/*.c
    SOURCES += ../../dbi/*.c
#    SOURCES += ../../net/*.c
    SOURCES += ../../net/linux/*.c

    LIBS += -ldl
    LIBS += -lmysql
}

win32 {
    HEADERS += ..\\..\\include\\cwt\\*.h
    HEADERS += ..\\..\\include\\cwt\\algo\\*.h
    HEADERS += ..\\..\\include\\cwt\\algo\\*.h
    HEADERS += ..\\..\\include\\cwt\\io\\*.h
    HEADERS += ..\\..\\include\\cwt\\stylex\\*.h
    HEADERS += ..\\..\\include\\cwt\\dbi\\*.h
    HEADERS += ..\\..\\include\\cwt\\net\\*.h

    SOURCES += ..\\..\\*.c
    SOURCES += ..\\..\\algo\\*.c
    SOURCES += ..\\..\\event\\*.c
    SOURCES += ..\\..\\io\\*.c
    SOURCES += ..\\..\\io\\serial\\*.c
    SOURCES += ..\\..\\io\\serial\\win\\*.c
    SOURCES += ..\\..\\stylex\\*.c
    SOURCES += ..\\..\\dbi\\*.c
#    SOURCES += ..\\..\\net\\*.c
    SOURCES += ..\\..\\net\\win\\*.c

    LIBS += Advapi32.lib
    LIBS += ws2_32.lib
    LIBS += libmysql.lib
    LIBS += iphlpapi.lib
}

#message($$SOURCES)
#for(o, OBJECTS):message($${o})
