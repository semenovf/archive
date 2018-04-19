#!/bin/sh

for i in `ls -1` ; do
    if [ -d $i ] ; then
	cd $i

	if [ -d .git ] ; then
	    echo ""
	    echo "***Checking status in $i ..."
	    echo ""
	    git status
	fi

	cd ..
    fi
done