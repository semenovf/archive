include(common.pri)
message(Process common-app.pri)

DESTDIR=$$CWT_DESTDIR_APP

win32 {
#   !!!Overidde value provided by qmake.conf
    # Code Generation: runtime library MT-debug
#    debug: QMAKE_CXXFLAGS_DEBUG += /MDd 
#    release: QMAKE_CXXFLAGS_DEBUG += /MD
}

