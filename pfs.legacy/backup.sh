#!/bin/sh

DATE=`date +%Y%m%d`
PREFIX=`pwd`
PREFIX=`basename $PREFIX`
mkdir -p ../backup/$PREFIX/
tar -cjvf ../backup/$PREFIX/${PREFIX}-${DATE}.tar.bz2 ./
