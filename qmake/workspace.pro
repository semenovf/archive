TEMPLATE = subdirs
CONFIG  += ordered

SUBDIRS += pfs-test
pfs-test.file = pfs-test/.gbs/solution.pro

SUBDIRS += pfs
pfs.file = pfs/.gbs/solution.pro

SUBDIRS += pfs-sys
pfs-sys.file = pfs-sys/.gbs/solution.pro

SUBDIRS += pfs-settings
pfs-settings.file = pfs-settings/.gbs/solution.pro

SUBDIRS += pfs-json
pfs-json.file = pfs-json/.gbs/solution.pro

SUBDIRS += pfs-settings-json
pfs-settings-json.file = pfs-settings-json/.gbs/solution.pro

SUBDIRS += pfs-app
pfs-app.file = pfs-app/.gbs/solution.pro

SUBDIRS += pfs-io
pfs-io.file = pfs-io/.gbs/solution.pro

SUBDIRS += pfs-io-serial
pfs-io-serial.file = pfs-io-serial/.gbs/solution.pro

#    SUBDIRS += pfs-io-ipx
