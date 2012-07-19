include(config.pri)
message(Process common.pri)

QT=
OBJECTS_DIR = $$CWT_CONFIG/$$CWT_TARGET_PLATFORM/$$CWT_TARGET_CPU


unix {

# CXX flags
    QMAKE_CFLAGS += -O0 -Werror
    QMAKE_CXXFLAGS += -O0 -Werror

# linker flags
    LIBS += -L$$CWT_DESTDIR_LIB
# add current directory to shared files search path
#    LIBS += -R ./
    LIBS += -Xlinker -rpath=.
}

win32 {

    INCLUDEPATH   += $$CWT_ROOT_DIR\\include

# CXX flags
    DEFINES += _CRT_SECURE_NO_WARNINGS # disbale 'depricated' warning
    debug: QMAKE_CXXFLAGS += /Od
    QMAKE_CXXFLAGS += /EHs /EHc 


# en: /ZI parameter applied only for x86 target architecture while compiling
# and it is not applied for x64 and Itanium (IPF).
#
# ru: Параметр /ZI доступен только при компиляции для целевой архитектуры x86; 
# этот параметр компилятора недоступен в компиляторах 
# для целевых платформ x64 или семейства процессоров Itanium (IPF).

#   !!!Overidde value provided by qmake.conf
#    QMAKE_CXXFLAGS_DEBUG  = -Zi # /ZI

    debug: QMAKE_CFLAGS_DEBUG = /ZI /MDd 
    release: QMAKE_CFLAGS_DEBUG = /ZI /MD

    debug: QMAKE_CXXFLAGS_DEBUG = /ZI /MDd 
    release: QMAKE_CXXFLAGS_DEBUG = /ZI /MD


# linker flags
    QMAKE_LFLAGS += /map

    LIBS += /LIBPATH:$$CWT_DESTDIR_LIB
    LIBS += /LIBPATH:$$CWT_EXTLIBDIR\\$$CWT_CONFIG
    LIBS += /LIBPATH:$$CWT_EXTLIBDIR\\$$CWT_TARGET_PLATFORM\\$$CWT_TARGET_CPU
}


x64 {
    debug: QMAKE_CFLAGS_DEBUG = /Zi /MDd 
    release: QMAKE_CFLAGS_DEBUG = /Zi /MD
    
    debug: QMAKE_CXXFLAGS_DEBUG = /Zi /MDd 
    release: QMAKE_CXXFLAGS_DEBUG = /Zi /MD
}
