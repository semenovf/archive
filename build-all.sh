#!/bin/sh

DIRS="cwt-core cwt-io cwt-logger cwt-uri cwt-csv cwt-json cwt-serial cwt-fs cwt-app"

for dir in $DIRS ; do
    cd $dir
    ./build.sh
    if [ $? -ne 0 ] ; then exit 1; fi
    cd ..
done