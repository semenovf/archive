CWT_CONFIG += debug
!contains(CWT_CONFIG, no-strict) {
    CWT_CONFIG += strict
}

INCLUDEPATH = $$(HOME)/include
#CWT_CONFIG += gprof

unix: CWT_ROOT_DIR = $$(HOME)/ws
win32: CWT_ROOT_DIR = \ws
