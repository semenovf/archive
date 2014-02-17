#!/bin/sh

#
# Install: copy this file into directory with test executables
# Usage: simply run this script from tests' directory.
#

PREFIX=
SUFFIX=.err
FAILED=
OUTDIR=output

if [ ! -d $OUTDIR ] ; then
    mkdir $OUTDIR
fi

rm -f ${OUTDIR}/${PREFIX}*${SUFFIX}

for i in test_* ; do
    OUTPUT=${OUTDIR}/${PREFIX}${i}${SUFFIX}
    printf "%-30s ... " $i
    ./$i > $OUTPUT 2>&1

    if [ $? != 0 ] ; then
	printf "[FAIL]\n"
	FAILED="yes"
    else
	rm $OUTPUT
	printf "[OK]\n"
    fi
done

if [ -n $FAILED ] ; then
    echo "========================"
    echo "ERROR: some tests failed"
    exit 1
fi

exit 0


