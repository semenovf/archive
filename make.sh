#!/bin/sh

#MULTITHREAD=-j
MULTITHREAD=

usage()
{
  echo "Usage:"
  echo "$0 [all | clean | deploy | install | PROJECT_NAME]"
  echo ""
  echo "Actions:"
  echo "all        build targets"
  echo "clean      clean targets"
  echo "deploy"
  echo "| install  deploy (install) targets, header files, documentation"
}

backup()
{
  DATE=`date +%Y%m%d`
  PREFIX=`pwd`
  tar -cjvf ${PREFIX}-${DATE}.tar.bz2 ./
}

ACTION=usage
if [ -n "$1" ] ; then
    ACTION=$1
fi

case $ACTION in
  clean)
    gbs project --clean
    ;;
  all|build)
    gbs project --build

    if [ $? -ne 0 ] ; then
	exit 1
    fi

#    make install
    ;;
  deploy|install)
#    make install
    ;;
  backup)
    gbs project --clean
#    make distclean
    backup
    ;;
  cppcheck)
#    cppcheck
    ;;
  usage|help|*)
    usage
    exit 1
    ;;
  *)
    gbs project --build --name=$ACTION
    ;;
esac

exit 0


