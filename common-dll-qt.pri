include(common-dll.pri)
message(Process common-dll-qt.pri)

CONFIG += qt
QT += core

win32 {
#    !qmake defaults
#
#    QMAKE_CXXFLAGS += /nologo      # qmake provide
#    QMAKE_CXXFLAGS += /EHsc        # qmake provide
#    QMAKE_CXXFLAGS += /Zc:wchar_t- # qmake provide
}