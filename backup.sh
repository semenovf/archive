#!/bin/sh
./clean.sh
DATE=`date +%Y%m%d`
tar -cjvf cwt-core-$DATE.tar.bz2 ./
