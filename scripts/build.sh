#!/bin/sh
premake5 gmake --file=.gbs/solution.gbs
make -C .gbs $*
