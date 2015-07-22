#!/bin/sh
premake5 gmake --file=.gbs/solution.gbs --fatal
make -C .gbs all config=debug_unix64
