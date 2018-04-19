#!/bin/sh
./clean.sh
DATE=`date +%Y%m%d`
tar -cjvf ../cwt-$DATE.tar.bz2 ./
