include(../common.pri)
TEMPLATE = lib
TARGET = jq-sc

unix {
    LIBS += -L../../jq/build 
    LIBS += -L/home/wladt/lib
    LIBS += -lpcsclite 
    LIBS += -ljq 
}

win32 {
#	DEFINES += _CRT_SECURE_NO_WARNINGS
	CONFIG += dll
	DEFINES += DLL_EXPORTS
	LIBS += /LD
	LIBS += advapi32.lib
	LIBS += winscard.lib
	LIBS += /LIBPATH:..\\..\\jq\\build-win32
    LIBS += ../../jq/build-win32/jq.lib
}

HEADERS = ../jq/*.hpp
SOURCES = *.cpp
