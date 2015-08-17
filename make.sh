#!/bin/sh
ACTION=$1
shift
gbs project --$ACTION $*
