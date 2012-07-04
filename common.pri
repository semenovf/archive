include(config.pri)
message(Process common.pri)

QT=
OBJECTS_DIR = $$CWT_CONFIG/$$CWT_TARGET_PLATFORM/$$CWT_TARGET_CPU


unix {

# CXX flags
    QMAKE_CXXFLAGS += -O0

# linker flags
    LIBS += -L$$CWT_DESTDIR_LIB
}

win32 {

    INCLUDEPATH   += $$CWT_ROOT_DIR\\include

# CXX flags
    DEFINES += _CRT_SECURE_NO_WARNINGS # disbale 'depricated' warning
    debug: QMAKE_CXXFLAGS += /Od
    QMAKE_CXXFLAGS += /EHs /EHc 


# ѕараметр /ZI доступен только при компил€ции дл€ целевой архитектуры x86; 
# этот параметр компил€тора недоступен в компил€торах 
# дл€ целевых платформ x64 или семейства процессоров Itanium (IPF).

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
