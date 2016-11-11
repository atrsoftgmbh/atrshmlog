#!/bin/bash
#!/usr/bin/ksh
# $Id:$
#
# we get the files from main and populate to all includes
#
# this is generic so far

if [ -z $ATRSHMLOG_PLATFORM ]
then
    echo "please set the environment via dot.platform.sh"
    exit 1
fi

case $ATRSHMLOG_PLATFORM in
    linux)
	# linux x86_64 gnu
	LIB=libatrshmlog.a
	;;

    cygwin)
	# cygwin x86_64 gnu
	LIB=libatrshmlog.a
	;;

    mingw)
	# mingw x86_64 gnu via cygwin
	LIB=libatrshmlog.a
	;;

    *)

	echo "no platform found. i gave up."
	exit 1
	;;
esac


cp ../src/atrshmlog.h src/includes/atrshmlog.h
cp ../src/atrshmlog_internal.h src/includes/atrshmlog_internal.h
cp ../src/$LIB src/$LIB


# end of file




