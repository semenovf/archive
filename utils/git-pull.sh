#!/bin/sh

DIRS=`ls -1`

for dir in $DIRS ; do
    if [ -d $dir ] ; then
	if [ -d $dir/.git ] ; then
	    echo Git pulling for $dir ...
	    cd $dir
	    git pull --all
	    if [ "$?" != "0" ] ; then
		echo Git pulling failed. >&2
		exit 1
	    fi
	    cd ..
	    echo Git pulling for $dir is OK.
	fi
    fi
done