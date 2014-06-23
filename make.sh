#!/bin/sh

#MULTITHREAD=-j
MULTITHREAD=

if [ -z $GBS_HOME ]; then
    echo "Error: GBS_HOME is not set, point it to the correct path of Griotte build system" >&2
    exit 1
fi


usage()
{
  echo Usage
  echo $0 \[all \| clean\]
}


case $1 in
  clean)
    qmake -makefile -r -o Makefile
    make distclean
    ;;
  all)
    qmake -makefile -r -o Makefile
    make $MULTITHREAD all
    if [ $? -ne 0 ] ; then exit 1 ; fi
    ;;
  *)
    usage
    exit 1
esac

exit 0


