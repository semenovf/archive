#!/bin/bash

SOLUTIONS=`cat solutions.txt`
DATE=`date +%Y%m%d`
TAR_NAME=`pwd`
TAR_NAME=`basename ${TAR_NAME}`
TAR_NAME="${TAR_NAME}-${DATE}.tar"

#tar -cjvf ../${PREFIX}-${DATE}.tar.bz2 ./


for i in $SOLUTIONS ; do
    if [ ${i:0:1} != "#" ] ; then
	tar --append --exclude=.git -vf $TAR_NAME $i
    fi
done

gzip -S .gz $TAR_NAME
