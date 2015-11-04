include(common-dll.pri)
#message(Process common-dll-qt.pri)

CONFIG += qt
QT += core
INCLUDEPATH += $$(QT_HOME)/include
DEFINES += PFS_HAVE_QT

unix {
    LIBS += -L$$(QT_HOME)/lib
}

win32 {
}


win32 {
#    !qmake defaults
#
#    QMAKE_CXXFLAGS += /nologo      # qmake provide
#    QMAKE_CXXFLAGS += /EHsc        # qmake provide
#    QMAKE_CXXFLAGS += /Zc:wchar_t- # qmake provide
}