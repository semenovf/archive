QT=
CONFIG = debug warn_on
OBJECTS_DIR = obj

INCLUDEPATH += ../include ../../jq/include /home/wladt/include /home/wladt/include/PCSC

unix {
	DESTDIR=../build
	QMAKE_CXXFLAGS += -O0
}
win32 {
	DESTDIR=../build-win32
	DEFINES += UNICODE DLL_EXPORTS
	QMAKE_CXXFLAGS += /Od /EHs /EHc 
}


