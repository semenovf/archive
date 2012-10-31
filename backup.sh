#!/bin/sh
./clean.sh
DATE=`date +%Y%m%d`
tar -cjvf cwt-abnf-$DATE.tar.bz2 ./
