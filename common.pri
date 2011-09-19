CONFIG += qt debug warn_on
OBJECTS_DIR = obj
QT += gui

INCLUDEPATH += ../include
INCLUDEPATH += ../../jq/include

unix {
	#INCLUDEPATH += $$(HOME)/projects/ws-jq/jq/include
	DESTDIR=../build
	QMAKE_CXXFLAGS += -O0
}
win32 {
	DESTDIR=../build-win32
	DEFINES += UNICODE
	QMAKE_CXXFLAGS += /Od /EHs /EHc 
}


