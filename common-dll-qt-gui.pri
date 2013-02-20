include(common-dll-qt.pri)
message(Process common-dll-qt-gui.pri)

QT += gui
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    QMAKE_LIBS_OPENGL     -= -lGL
    QMAKE_LIBS_OPENGL_QT  -= -lGL
}


#release: CONFIG += windows  # w/o console
##release: CONFIG += console  # w/o console
#debug  : CONFIG += console  # with console

