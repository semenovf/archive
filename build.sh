#!/bin/sh
cd .qmake
make distclean
del Makefile
qmake -makefile -r -o Makefile
make all

