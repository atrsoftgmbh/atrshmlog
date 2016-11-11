#!/bin/bash
#!/usr/bin/ksh
#
# $Id:$
#
# link the objects to an executable
#
# check for your platform.
# if you dont find it add a new one and sent me a postcard with it

if [ -z $ATRSHMLOG_PLATFORM ]
then
    echo "please set the environment via dot.platform.sh"
    exit 1
fi

case $ATRSHMLOG_PLATFORM in
    linux)
	# linux x86_64 gnu
	CC="gcc -pthread"
	LIBMODULE=-latrshmlog
	;;

    cygwin)
	# cygwin x86_64 gnu
	CC="gcc -pthread"
	LIBMODULE=-latrshmlog
	;;

    mingw)
	# mingw x86_64 gnu via cygwin - optional -pthread  -pthread for libpthread support
	CC="x86_64-w64-mingw32-gcc"
	LIBMODULE=-latrshmlog
	;;

    *)

	echo "no platform found. i gave up."
	exit 1
	;;
esac








###############################################

P=${1%%.o}

shift

$CC -L. ${P}.o $* $LIBMODULE -o $P

# end of file
