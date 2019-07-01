include(../common.pri)

TEMPLATE = app
TARGET = test
unix {
<<<<<<< HEAD
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
=======
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
>>>>>>> jq-logindialog-qt/master
