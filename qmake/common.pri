include(config.pri)
#message(Process common.pri)

debug: OBJECTS_DIR = debug/$$CWT_TARGET_PLATFORM/$$CWT_TARGET_CPU
release: OBJECTS_DIR = release/$$CWT_TARGET_PLATFORM/$$CWT_TARGET_CPU
# not used: DEFINED += CWT_TARGET_CPU_$$CWT_TARGET_CPU

unix {

# C/CXX flags
    QMAKE_CFLAGS += -O0
    QMAKE_CXXFLAGS += -O0

    contains(CWT_CONFIG, strict) {
	QMAKE_CFLAGS += -Werror
        QMAKE_CXXFLAGS += -Werror
    }

# linker flags
    LIBS += -L$$CWT_DESTDIR_LIB
# add current directory to shared files search path
#    LIBS += -R ./
    LIBS += -Xlinker -rpath=.
    LIBS += -L$$CWT_LIB_DIR

    INCLUDEPATH += $$CWT_H_DIR
}

win32 {
# CXX flags
    # disbale 'depricated' warning
    DEFINES += _CRT_SECURE_NO_WARNINGS 
    DEFINES += _CRT_NONSTDC_NO_DEPRECATE

    debug: QMAKE_CXXFLAGS += /Od
    QMAKE_CXXFLAGS += /EHs /EHc

   # Treat all compiler warnings as errors.
    contains(CWT_CONFIG, strict) {
        QMAKE_CXXFLAGS += /WX
    }

# en: /ZI parameter applied only for x86 target architecture while compiling
# and it is not applied for x64 and Itanium (IPF).
#
# ru: Параметр /ZI доступен только при компиляции для целевой архитектуры x86; 
# этот параметр компилятора недоступен в компиляторах 
# для целевых платформ x64 или семейства процессоров Itanium (IPF).

# /Za - compatible with either ANSI C or ANSI C++
# disable it because it cause the "warning error C2008: '$' : unexpected in macro definition"
#
#   !!!Overidde value provided by qmake.conf
#    QMAKE_CXXFLAGS_DEBUG  = -Zi # /ZI

    debug: QMAKE_CFLAGS_DEBUG = /ZI /MDd
    release: QMAKE_CFLAGS_DEBUG = /ZI /MD

    debug: QMAKE_CXXFLAGS_DEBUG = /ZI /MDd
    release: QMAKE_CXXFLAGS_DEBUG = /ZI /MD


# linker flags
    QMAKE_LFLAGS += /map

    LIBS += /LIBPATH:$$CWT_DESTDIR_LIB
    LIBS += /LIBPATH:$$CWT_LIB_LIB
    LIBS += /LIBPATH:$$CWT_EXTLIBDIR\\$$CWT_TARGET_BUILD
    LIBS += /LIBPATH:$$CWT_EXTLIBDIR\\$$CWT_TARGET_PLATFORM\\$$CWT_TARGET_CPU

    INCLUDEPATH += $$CWT_H_DIR
}


x64 {
    debug: QMAKE_CFLAGS_DEBUG = /Zi /MDd /WX
    release: QMAKE_CFLAGS_DEBUG = /Zi /MD /WX

    debug: QMAKE_CXXFLAGS_DEBUG = /Zi /MDd /WX
    release: QMAKE_CXXFLAGS_DEBUG = /Zi /MD /WX
}
