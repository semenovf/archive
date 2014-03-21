#!/bin/sh

#
# Install: copy this file into directory with test executables
# Usage: simply run this script from tests' directory.
#

PREFIX=
SUFFIX=.err
FAILED=no
OUTDIR=output

if [ ! -d $OUTDIR ] ; then
    mkdir $OUTDIR
fi

rm -f ${OUTDIR}/${PREFIX}*${SUFFIX}

for i in test_* ; do
    OUTPUT=${OUTDIR}/${PREFIX}${i}${SUFFIX}
    printf "%-30s ... " $i
    ./$i > $OUTPUT 2>&1

    case $? in
    0)
	rm $OUTPUT
	printf "[OK]\n"
	;;
    1)
	printf "[FAIL]\n"
	FAILED="yes"
	;;
    2)
	printf "[INCOMPLETE]\n"
	FAILED="yes"
	;;
    esac
done

if [ "$FAILED" = "yes" ] ; then
    echo "========================"
    echo "ERROR: some tests failed or incomplete"
    exit 1
fi

exit 0


