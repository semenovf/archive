#!/bin/sh
./clean.sh
DATE=`date +%Y%m%d`
tar -cjvf cwt-sqlite3-$DATE.tar.bz2 ./
