#!/bin/sh
./clean.sh
DATE=`date +%Y%m%d`
PREFIX=`pwd`
tar -cjvf ${PREFIX}-${DATE}.tar.bz2 ./
