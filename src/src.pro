include(../common.pri)
TEMPLATE = lib
TARGET   = jq-logindialog-qt

unix {
	LIBS += -L/home/wladt/lib
	LIBS += -ljq
}

win32 {
	DEFINES += DLL_EXPORTS
    CONFIG += dll

	LIBS += /LD
	LIBS += /LIBPATH:..\\..\\jq\\build-win32
    LIBS += ../../jq/build-win32/jq.lib
}

HEADERS = ../include/jq/qt/*.hpp
SOURCES = *.cpp
RESOURCES = images.qrc
