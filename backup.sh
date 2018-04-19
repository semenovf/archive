#!/bin/sh

DATE=`date +%Y%m%d`
PREFIX=`pwd`
PREFIX=`basename $PREFIX`
tar -cjvf ../${PREFIX}-${DATE}.tar.bz2 ./
