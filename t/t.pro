include(../common.pri)

TEMPLATE = app
TARGET = test
unix {
	LIBS +=  -L../build
	LIBS += -ljq-logindialog-qt
    HEADERS += *.t
    SOURCES += *.cpp
}

win32 {
#    LIBS += Advapi32.lib build-win32/jq.lib -L../build-win32
    LIBS += -ljq -L../build-win32
    HEADERS += *.t ../include/*.hpp
    SOURCES += *.cpp
}
