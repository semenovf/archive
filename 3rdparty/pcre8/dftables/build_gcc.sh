#!/bin/sh

gcc -I../ -I../../../../pfs/include -DHAVE_CONFIG_H dftables.c -o dftables
./dftables ../dftables.c
rm dftables

