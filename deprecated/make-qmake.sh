#!/bin/sh

#MULTITHREAD=-j
MULTITHREAD=

if [ -z $GBS_HOME ]; then
    echo "Error: GBS_HOME is not set, point it to the correct path of Griotte build system" >&2
    exit 1
fi


usage()
{
  echo "Usage:"
  echo "$0 [all | clean | deploy | install]"
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

cppcheck()
{
  # Cppcheck command arguments
  VERBOSE=--verbose
  #PLATFORM=--platform=unix32
  PLATFORM=
  PROGRESS=--report-progress
  TEMPLATE=--template=gcc

  INCLUDES_FILE=cppcheck/includes-file
  SOURCES_FILE=cppcheck/sources-file
  OPTIONS_FILE=cppcheck/options

  if [ ! -f $INCLUDES_FILE ] ; then
       echo "ERROR: $dir: No '$INCLUDES_FILE' found" 1>&2
  elif [ ! -f $SOURCES_FILE ] ; then
      echo "ERROR: $dir: No '$SOURCES_FILE' found" 1>&2
  else
      OPTIONS=
      if [ -f $OPTIONS_FILE ] ; then
          OPTIONS=`cat $OPTIONS_FILE`
      fi
      echo "===== CPPCHECK: $PROJECT ====="
      cppcheck --force $VERBOSE $PLATFORM $TEMPLATE $PROGRESS $OPTIONS --enable=all --error-exitcode=1 \
          --includes-file=$INCLUDES_FILE \
          --file-list=$SOURCES_FILE

      if [ $? -ne 0 ] ; then 
          exit 1;
      fi
  fi
}

prepare_makefile()
{
    qmake -makefile -o Makefile
}

ACTION=usage
if [ -n "$1" ] ; then
    ACTION=$1
fi

case $ACTION in
  clean)
    prepare_makefile
    make distclean
    ;;
  all)
    prepare_makefile
    make $MULTITHREAD all

    if [ $? -ne 0 ] ; then
	exit 1
    fi

    make install
    ;;
  deploy|install)
    prepare_makefile
    make install
    ;;
  backup)
    prepare_makefile
    make distclean
    backup
    ;;
  cppcheck)
    cppcheck
    ;;
  usage|help|*)
    usage
    exit 1
esac

exit 0


