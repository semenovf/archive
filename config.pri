#
# Set this variable in global.pri file
#
#CWT_CONFIG = release | debug
include($$(GBS_PDIR)/global.pri)
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
QT=
CONFIG = $$CWT_CONFIG
CONFIG += warn_on
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

CONFIG(debug): CWT_TARGET_BUILD = debug
CONFIG(release): CWT_TARGET_BUILD = release

unix {
    isEmpty(CWT_DESTDIR_APP) {
	CWT_DESTDIR_APP = $$CWT_ROOT_DIR/build/$$CWT_TARGET_PLATFORM/$$CWT_TARGET_CPU/$$CWT_TARGET_BUILD
    }
    isEmpty(CWT_DESTDIR_LIB) {
	CWT_DESTDIR_LIB = $$CWT_ROOT_DIR/build/$$CWT_TARGET_PLATFORM/$$CWT_TARGET_CPU/$$CWT_TARGET_BUILD
    }
    INCLUDEPATH    += $$CWT_ROOT_DIR/include
#    QMAKE_LINK = $$QMAKE_LINK_C
}

win32 {
    isEmpty(CWT_DESTDIR_APP) {
	CWT_DESTDIR_APP = $$CWT_ROOT_DIR\\build\\$$CWT_TARGET_PLATFORM\\$$CWT_TARGET_CPU\\$$CWT_TARGET_BUILD
    }
    isEmpty(CWT_DESTDIR_LIB) {
	CWT_DESTDIR_LIB = $$CWT_ROOT_DIR\\build\\$$CWT_TARGET_PLATFORM\\$$CWT_TARGET_CPU\\$$CWT_TARGET_BUILD
    }
#    CWT_EXTLIBDIR   = $$CWT_ROOT_DIR\\extlib
}

contains(CWT_CONFIG, gprof) {
    contains(CWT_CONFIG, debug) {
	QMAKE_CFLAGS_DEBUG += -pg
        QMAKE_CXXFLAGS_DEBUG += -pg
	QMAKE_LFLAGS_DEBUG += -pg
    }
}

#message(Target Platform : $${CWT_TARGET_PLATFORM})
#message(Target CPU      : $${CWT_TARGET_CPU})
#message(Target Build    : $${CWT_TARGET_BUILD})
#message(Workspace folder: $${CWT_ROOT_DIR})
#message(CONFIG          : $${CONFIG})
#message(QMAKESPEC       : $$QMAKESPEC)
#message(QT              : $${QT})
#message(Processed config.pri)
