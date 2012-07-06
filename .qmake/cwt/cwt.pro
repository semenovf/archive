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

    SOURCES += ../../src/*.c
    SOURCES += ../../src/algo/*.c
    SOURCES += ../../src/event/*.c
    SOURCES += ../../src/io/*.c
    SOURCES += ../../src/stylex/*.c
    SOURCES += ../../src/dbi/*.c
#    SOURCES += ../../src/net/*.c
    SOURCES += ../../src/net/linux/*.c

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

    SOURCES += ..\\..\\src\\*.c
    SOURCES += ..\\..\\src\\algo\\*.c
    SOURCES += ..\\..\\src\\event\\*.c
    SOURCES += ..\\..\\src\\io\\*.c
    SOURCES += ..\\..\\src\\io\\serial\\*.c
    SOURCES += ..\\..\\src\\io\\serial\\win\\*.c
    SOURCES += ..\\..\\src\\stylex\\*.c
    SOURCES += ..\\..\\src\\dbi\\*.c
#    SOURCES += ..\\..\\src\\net\\*.c
    SOURCES += ..\\..\\src\\net\\win\\*.c

    LIBS += Advapi32.lib
    LIBS += ws2_32.lib
    LIBS += libmysql.lib
    LIBS += iphlpapi.lib
}

#message($$SOURCES)
#for(o, OBJECTS):message($${o})
