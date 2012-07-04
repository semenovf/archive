include(common.pri)
message(Process common-petaloid.pri)

TEMPLATE = lib
CONFIG += dll
DESTDIR=$$CWT_DESTDIR_LIB

win32 {
#    DEFINES += DLL_EXPORTS
}
