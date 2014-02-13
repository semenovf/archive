include(common.pri)
#message(Process common-dll.pri)


TEMPLATE = lib
CONFIG += dll
DESTDIR=$$CWT_DESTDIR_LIB

win32 {
    DEFINES += DLL_EXPORTS

#   !!!Overidde value provided by qmake.conf
    # Code Generation: runtime library MT-debug
#    debug:   QMAKE_CXXFLAGS_DEBUG += /MDd 
#    release: QMAKE_CXXFLAGS_DEBUG += /MD

}
