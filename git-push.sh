#!/bin/sh

DIRS=`ls -1`

for dir in $DIRS ; do
    if [ -d $dir ] ; then
	if [ -d $dir/.git ] ; then
	    echo Git pushing for $dir ...
	    cd $dir
	    git add -A
	    git commit
	    git push
	    if [ "$?" != "0" ] ; then
		echo Git pushing failed. >&2
		exit 1
	    fi
	    cd ..
	    echo Git pulling for $dir is OK.
	fi
    fi
done