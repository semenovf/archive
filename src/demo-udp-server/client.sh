#!/bin/bash

INTERVAL=0.0001
COUNTER=0

while [ 1 ] ; do
    echo -n "hello ${COUNTER}" | nc -w 0 -u -4 127.0.0.1 10000

    # Sleep
    perl -e "select(undef,undef,undef,${INTERVAL})"

    COUNTER=$((COUNTER+1))
done