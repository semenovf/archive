include($$(GBS_HOME)/common-app.pri)

TARGET = uuid-gen
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../cwt-common/include

unix {
    SOURCES += ../../../src/uuid_c.c
    SOURCES += ../../../src/md5.c
    SOURCES += ../../../src/sha1.c
    SOURCES += ../../../src/mt.c
    SOURCES += ../../../utils/uuid_gen.c
}

win32 {
}
