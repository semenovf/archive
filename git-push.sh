#!/bin/sh

DIRS=`ls -1`

for dir in $DIRS ; do
    if [ -d $dir ] ; then
	if [ -d $dir/.git ] ; then
	    echo Git pushing for $dir ...
	    cd $dir
	    if [ -x clean.sh ] ; then
		echo Cleaning ...
		./clean.sh
	    fi
	    git add -A
	    git commit
	    sleep 1
	    git push
	    if [ "$?" != "0" ] ; then
		echo Git pushing failed. >&2
		exit 1
	    fi
	    cd ..
	    echo Git pushing for $dir is OK.
#        sleep 5
	fi
    fi
done