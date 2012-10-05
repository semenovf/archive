#!/bin/sh
./clean.sh
DATE=`date +%Y%m%d`
tar -cjvf blc-$DATE.tar.bz2 ./
