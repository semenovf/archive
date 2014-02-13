#!/bin/sh

DIRS="cwt-test pfs cwt-sys cwt-app cwt-dom cwt-xml cwt-debby cwt-debby-sqlite3 cwt-io cwt-griotte"

for dir in $DIRS ; do
    cd $dir
    ./build.sh
    if [ $? -ne 0 ] ; then exit 1; fi
    cd ..
done