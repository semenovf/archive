#!/bin/sh

DIRS=`ls -1`

for dir in $DIRS ; do
    if [ -d $dir ] ; then
	if [ -d $dir/.git ] ; then
	    echo ===========================================================
	    echo Git pushing for $dir ...
	    cd $dir
	
	    if [ -f .gbs/solution.gbs ] ; then
		echo Cleaning ...
		gbs clean
	    fi

	    git add -A && git commit
	    git push

	    if [ "$?" != "0" ] ; then
		echo Git pushing failed. >&2
		exit 1
	    fi
	    cd ..
	    echo Git pushing for $dir is OK.
	fi
    fi
done