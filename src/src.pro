include(../common.pri)
TEMPLATE = lib
TARGET = jq-sc
unix {
    LIBS += -ljq -lpcsclite -L../../jq/build -L/home/wladt/lib
}
win32 {
	DEFINES += _CRT_SECURE_NO_WARNINGS
#	LIBS += Advapi32.lib /LD
	CONFIG += dll
}

HEADERS = ../jq/*.hpp
SOURCES = *.cpp
