<<<<<<< HEAD
QT=
CONFIG = debug warn_on
OBJECTS_DIR = obj

INCLUDEPATH += ../include
INCLUDEPATH += ../../jq/include 


unix {
	INCLUDEPATH += /home/wladt/include/PCSC
	DEFINES += PCSCD PCSCLITE_CSOCK_NAME=\\\"/var/run/pcscd/pcscd.comm\\\"
	DESTDIR=../build
	QMAKE_CXXFLAGS += -O0
}

win32 {
	DESTDIR=../build-win32
	DEFINES += DLL_EXPORTS
	QMAKE_CXXFLAGS += /Od /EHs /EHc 
}
=======
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


>>>>>>> jq-logindialog-qt/master
