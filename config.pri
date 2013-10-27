#
# Set this variable in project's .pro file
#
#CWT_CONFIG = release | debug
isEmpty(CWT_CONFIG) { CWT_CONFIG = debug }

CWT_TARGET_PLATFORM=$$(CWT_TARGET_PLATFORM)
CWT_TARGET_CPU=$$(CWT_TARGET_CPU)

#
# TARGET_PLATFORM variable is set in WinSDK's SetEnv
#
isEmpty(CWT_TARGET_PLATFORM) { CWT_TARGET_PLATFORM=$$(TARGET_PLATFORM) }
win32: isEmpty(CWT_TARGET_PLATFORM) { CWT_TARGET_PLATFORM=win32 }
unix: isEmpty(CWT_TARGET_PLATFORM)  { CWT_TARGET_PLATFORM=unix }

win32: CWT_UNICODE=yes

#
# TARGET_CPU variable is set in WinSDK's SetEnv
#
isEmpty(CWT_TARGET_CPU) { CWT_TARGET_CPU=$$(TARGET_CPU) }
isEmpty(CWT_TARGET_CPU) { CWT_TARGET_CPU=x86 }

isEmpty(CWT_UNICODE)    { DEFINES -= UNICODE }

# qmake internal variables
QT-=gui
QT-=webkit
CONFIG = $$CWT_CONFIG
CONFIG += warn_on
CONFIG += resources
CONFIG += $$CWT_TARGET_CPU
CONFIG += $$CWT_TARGET_PLATFORM

DEFINES += CWT_LANG=$$CWT_LANG
DEFINES += CWT_TARGET_PLATFORM=$$CWT_TARGET_PLATFORM

# Multi-threading support
DEFINES += CWT_MT

# Comment line below to set unicode character size to 2 bytes
# Note. Only after self implementation of String class!
#DEFINES += CWT_UCS4_CHAR

unix: CWT_ROOT_DIR = $$(HOME)
win32: CWT_ROOT_DIR = \

unix {
    CWT_DESTDIR_APP = $$CWT_ROOT_DIR/build/$$CWT_TARGET_PLATFORM/$$CWT_TARGET_CPU/$$CWT_CONFIG
    CWT_DESTDIR_LIB = $$CWT_ROOT_DIR/build/$$CWT_TARGET_PLATFORM/$$CWT_TARGET_CPU/$$CWT_CONFIG
    INCLUDEPATH    += $$CWT_ROOT_DIR/include
#    QMAKE_LINK = $$QMAKE_LINK_C
}

win32 {
    CWT_DESTDIR_APP = $$CWT_ROOT_DIR\\build\\$$CWT_TARGET_PLATFORM\\$$CWT_TARGET_CPU\\$$CWT_CONFIG
    CWT_DESTDIR_LIB = $$CWT_ROOT_DIR\\build\\$$CWT_TARGET_PLATFORM\\$$CWT_TARGET_CPU\\$$CWT_CONFIG
    CWT_EXTLIBDIR   = $$CWT_ROOT_DIR\\extlib
}


#message(Target Platform : $${CWT_TARGET_PLATFORM})
#message(Target CPU      : $${CWT_TARGET_CPU})
#message(Configuration   : $${CWT_CONFIG})
#message(Workspace folder: $${CWT_ROOT_DIR})
#message(CONFIG          : $${CONFIG})
#message(QMAKESPEC       : $$QMAKESPEC)
#message(QT              : $${QT})
#message(Processed config.pri)
