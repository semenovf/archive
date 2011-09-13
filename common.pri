QT = gui
CONFIG += qt debug warn_on
OBJECTS_DIR = obj
CONFIG += qt

INCLUDEPATH += ../include

unix {
	INCLUDEPATH += $$(HOME)/projects/ws-jq/jq/include
	LIBS +=  -L$$(HOME)/lib
	LIBS += -ljq-sc
	DESTDIR=../build
	QMAKE_CXXFLAGS += -O0
}
win32 {
	DESTDIR=../build-win32
	DEFINES += UNICODE DLL_EXPORTS
	QMAKE_CXXFLAGS += /Od /EHs /EHc 
}


