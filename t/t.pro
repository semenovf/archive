include(../common.pri)

TEMPLATE = app
TARGET = test
unix {
	LIBS +=  -L../build
	LIBS += -ljq-logindialog-qt
}

win32 {
	CONFIG += console #windows # Windows GUI application
	LIBS += /LIBPATH:..\\..\\jq\\build-win32
    LIBS += ../../jq/build-win32/jq.lib
	LIBS += /LIBPATH:..\\build-win32
    LIBS += ../build-win32/jq-logindialog-qt.lib
}

HEADERS += *.t 
HEADERS += ../include/jq/qt/*.hpp
SOURCES += *.cpp
