#!/bin/sh

ACTION=all
if [ -n "$1" ] ; then
    ACTION=$1
fi

case $ACTION in
  clean)
    gbs project --clean
    ;;
  all)
    gbs project --build
    ;;
  *)
    gbs project --build $ACTION
    ;;
esac

exit 0


