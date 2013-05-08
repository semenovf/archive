include(common-petaloid-qt.pri)
message(Process common-petaloid-qt-gui.pri)

QT += gui
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    QMAKE_LIBS_OPENGL     -= -lGL
    QMAKE_LIBS_OPENGL_QT  -= -lGL
}
