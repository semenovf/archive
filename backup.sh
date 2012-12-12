#!/bin/sh
./clean.sh
DATE=`date +%Y%m%d`
tar -cjvf cwt-mysql5-$DATE.tar.bz2 ./
