#!/bin/sh

DIRS="cwt-core cwt-io cwt-uri cwt-json cwt-fs cwt-csv cwt-app"

for dir in $DIRS ; do
    cd $dir
    ./build.sh
    if [ $? -ne 0 ] ; then exit 1; fi
    cd ..
done