#!/bin/sh

gcc -I../../include -I../../../cwt-common/include -DHAVE_CONFIG_H dftables.c -o dftables
./dftables ../dftables.c
rm dftables

