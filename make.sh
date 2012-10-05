#!/bin/sh

usage()
{
  echo Usage
  echo $0 \[all \| clean\]
}


case $1 in
  clean)
    make distclean
    rm Makefile
    qmake -makefile -r -o Makefile
    ;;
  all)
    qmake -makefile -r -o Makefile
    make all
    ;;
  *)
    usage
    exit 1
esac

exit 0


