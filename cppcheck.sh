#!/bin/sh

#DIRS="cwt-test pfs cwt-sys cwt-app cwt-dom cwt-xml cwt-debby cwt-debby-sqlite3 cwt-io cwt-griotte"
DIRS="pfs"

# Cppcheck command arguments
VERBOSE=--verbose
#PLATFORM=--platform=unix32
PLATFORM=
PROGRESS=--report-progress
TEMPLATE=--template=gcc

INCLUDES_FILE=cppcheck/includes-file
SOURCES_FILE=cppcheck/sources-file
OPTIONS_FILE=cppcheck/options

for dir in $DIRS ; do
    PROJECT=$dir
    cd $dir

    if [ ! -f $INCLUDES_FILE ] ; then
	echo "ERROR: $dir: No '$INCLUDES_FILE' found" 1>&2
    elif [ ! -f $SOURCES_FILE ] ; then
	echo "ERROR: $dir: No '$SOURCES_FILE' found" 1>&2
    else
	OPTIONS=
	if [ -f $OPTIONS_FILE ] ; then
	    OPTIONS=`cat $OPTIONS_FILE`
	fi
	echo "===== CPPCHECK: $PROJECT ====="
	cppcheck --force $VERBOSE $PLATFORM $TEMPLATE $PROGRESS $OPTIONS --enable=all --error-exitcode=1 \
		--includes-file=$INCLUDES_FILE \
		--file-list=$SOURCES_FILE
        if [ $? -ne 0 ] ; then exit 1; fi
    fi

    cd ..
done