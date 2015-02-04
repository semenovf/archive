include(common-petaloid.pri)
#message(Process common-petaloid-qt.pri)

CONFIG += qt
QT += core
INCLUDEPATH += $$(QT_HOME)/include
DEFINES += PFS_HAVE_QT

unix {
    LIBS += -L$$(QT_HOME)/lib
}

win32 {
}

