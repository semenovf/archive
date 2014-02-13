include($$(GBS_HOME)/qmake/common-app.pri)
TARGET = simple-clock-indicator
#INCLUDEPATH += /usr/include/libappindicator3-0.1

SOURCES += ../../src/*.c

unix {
    GTK_CFLAGS    = $$system(pkg-config --cflags gtk+-3.0)
    GTK_LIBS      = $$system(pkg-config --libs gtk+-3.0)
    APPINDICATOR_CFLAGS = $$system(pkg-config --cflags appindicator3-0.1)
    APPINDICATOR_LIBS = $$system(pkg-config --libs appindicator3-0.1)

    QMAKE_CFLAGS += $${GTK_CFLAGS} $${APPINDICATOR_CFLAGS}
    LIBS += $${GTK_LIBS}
    LIBS += $${APPINDICATOR_LIBS}
}
