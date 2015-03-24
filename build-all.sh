#!/bin/sh

DIRS="pfs-test pfs pfs-sys pfs-app pfs-dom pfs-xml pfs-debby pfs-debby-sqlite3 pfs-io pfs-griotte"

for dir in $DIRS ; do
    cd $dir
    ./build.sh
    if [ $? -ne 0 ] ; then exit 1; fi
    cd ..
done