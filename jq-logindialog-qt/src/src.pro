include(../common.pri)
TEMPLATE = lib
<<<<<<< HEAD
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
=======
TARGET   = jq-logindialog-qt

unix {
	LIBS += -L/home/wladt/lib
	LIBS += -ljq
}

win32 {
	DEFINES += DLL_EXPORTS
    CONFIG += dll

	LIBS += /LD
>>>>>>> jq-logindialog-qt/master
	LIBS += /LIBPATH:..\\..\\jq\\build-win32
    LIBS += ../../jq/build-win32/jq.lib
}

<<<<<<< HEAD
HEADERS = ../jq/*.hpp
SOURCES = *.cpp
=======
HEADERS = ../include/jq/qt/*.hpp
SOURCES = *.cpp
RESOURCES = images.qrc
>>>>>>> jq-logindialog-qt/master
