include(../common.pri)

TEMPLATE = app
TARGET = test
unix {
    LIBS += -ljq-sc -ljq -L../build -L../../jq/build
    HEADERS += *.t ../include/*.hpp
    SOURCES += *.cpp
}
win32 {
#    LIBS += Advapi32.lib build-win32/jq.lib -L../build-win32
    LIBS += -ljq-sc -L../build-win32
    HEADERS += *.t ../include/*.hpp
    SOURCES += *.cpp
}
