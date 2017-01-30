#!/bin/bash
#!/usr/bin/ksh
# $Id:$
#
# we get the files from main and populate to all included jdks
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
	LIB=libatrshmlogc.a
	;;

    cygwin)
	# cygwin x86_64 gnu
	LIB=libatrshmlogc.a
	;;

    mingw)
	# mingw x86_64 gnu via cygwin
	LIB=libatrshmlogc.a
	;;

    *)

	echo "no platform found. i gave up."
	exit 1
	;;
esac


find . -maxdepth 2 -mindepth 2 -type d -print | while read d rest
do

cp ../src/atrshmlog.h $d/src/includes/atrshmlog.h
cp ../src/atrshmlog_internal.h $d/src/includes/atrshmlog_internal.h
cp ../src/$LIB $d/src/$LIB

done

# end of file




