#!/bin/sh

#
# Default action is build
#
ACTION=all

if [ -n "$1" ] ; then
    ACTION=$1
fi

CUR_DIR=`pwd`
DIRS="pfs-test pfs pfs-sys pfs-io pfs-settings pfs-json pfs-settings-json pfs-app pfs-io-serial"

echo "ACTION=$ACTION"

for dir in $DIRS ; do
    cd $dir/.gbs

    $GBS_HOME/make.sh $ACTION

    if [ $? -ne 0 ] ; then
	cd $CUR_DIR
	exit 1
    fi

    cd $CUR_DIR
done
