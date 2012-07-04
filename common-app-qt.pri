include(common-app.pri)
message(Process common-app-qt.pri)

QT += core
CONFIG += qt

win32 {
#    !qmake defaults
#
#    QMAKE_CXXFLAGS += /nologo      # qmake provide
#    QMAKE_CXXFLAGS += /EHsc        # qmake provide
#    QMAKE_CXXFLAGS += /Zc:wchar_t- # qmake provide
}