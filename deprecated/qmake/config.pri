#
# Set this variable in global.pri file
#
#CWT_CONFIG = release | debug
#

include(find_file.pri)
include(prepend_include.pri)
GlobalPri = $$scanParentDirsToFindFile($$OUT_PWD, global.pri)

isEmpty(GlobalPri) {
    error("global.pri not found, it must be placed in workspace root directory")
} else {
    include($$GlobalPri)
}

!contains(CWT_CONFIG, debug) {
    !contains(CWT_CONFIG, release) {
	!contains(CWT_CONFIG, debug_and_release) {
	    !contains(CWT_CONFIG, debug_and_release_target) {
		CWT_CONFIG = debug
	    }
	}
    }
}

!contains(CWT_CONFIG, no-strict) {
    !contains(CWT_CONFIG, strict) {
	CWT_CONFIG += strict
    }
}

CWT_TARGET_PLATFORM=$$(CWT_TARGET_PLATFORM)
CWT_TARGET_CPU=$$(CWT_TARGET_CPU)

#
# TARGET_PLATFORM variable is set in WinSDK's SetEnv
#
isEmpty(CWT_TARGET_PLATFORM) { CWT_TARGET_PLATFORM=$$(TARGET_PLATFORM) }
win32: isEmpty(CWT_TARGET_PLATFORM) { CWT_TARGET_PLATFORM=win32 }
unix: isEmpty(CWT_TARGET_PLATFORM)  { CWT_TARGET_PLATFORM=unix }

# TARGET_CPU variable is set in WinSDK's SetEnv
#
isEmpty(CWT_TARGET_CPU) { CWT_TARGET_CPU=$$(TARGET_CPU) }
isEmpty(CWT_TARGET_CPU) { CWT_TARGET_CPU=$$QMAKE_HOST.arch }
isEmpty(CWT_TARGET_CPU) { CWT_TARGET_CPU=x86 }

# qmake internal variables
QT=
CONFIG = $$CWT_CONFIG
CONFIG += warn_on
CONFIG += $$CWT_TARGET_CPU
CONFIG += $$CWT_TARGET_PLATFORM

DEFINES += UNICODE
DEFINES += _UNICODE
DEFINES += CWT_TARGET_PLATFORM=$$CWT_TARGET_PLATFORM

# Multi-threading support
DEFINES += CWT_MT

CONFIG(debug): CWT_TARGET_BUILD = debug
CONFIG(release): CWT_TARGET_BUILD = release

unix {
    isEmpty(CWT_DESTDIR_APP) {
	CWT_DESTDIR_APP = $$CWT_WS_DIR/build/$$CWT_TARGET_PLATFORM/$$CWT_TARGET_CPU/$$CWT_TARGET_BUILD
    }

    isEmpty(CWT_DESTDIR_LIB) {
	CWT_DESTDIR_LIB = $$CWT_WS_DIR/build/$$CWT_TARGET_PLATFORM/$$CWT_TARGET_CPU/$$CWT_TARGET_BUILD
    }

    isEmpty(CWT_DESTDIR_TEST) {
	CWT_DESTDIR_TEST = $$CWT_WS_DIR/build/$$CWT_TARGET_PLATFORM/$$CWT_TARGET_CPU/$$CWT_TARGET_BUILD/tests
    }
}

win32 {
    isEmpty(CWT_DESTDIR_APP) {
	CWT_DESTDIR_APP = $$CWT_WS_DIR\\build\\$$CWT_TARGET_PLATFORM\\$$CWT_TARGET_CPU\\$$CWT_TARGET_BUILD
    }

    isEmpty(CWT_DESTDIR_LIB) {
	CWT_DESTDIR_LIB = $$CWT_WS_DIR\\build\\$$CWT_TARGET_PLATFORM\\$$CWT_TARGET_CPU\\$$CWT_TARGET_BUILD
    }

    isEmpty(CWT_DESTDIR_TEST) {
	CWT_DESTDIR_TEST = $$CWT_WS_DIR\\build\\$$CWT_TARGET_PLATFORM\\$$CWT_TARGET_CPU\\$$CWT_TARGET_BUILD\\tests
    }
}

contains(CWT_CONFIG, gprof) {
    contains(CWT_CONFIG, debug) {
	QMAKE_CFLAGS_DEBUG += -pg
        QMAKE_CXXFLAGS_DEBUG += -pg
	QMAKE_LFLAGS_DEBUG += -pg
    }
}
