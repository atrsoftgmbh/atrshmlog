#!/usr/local/bin/bash
#!/usr/bin/ksh
# $Id:$
#
# Compile the thing into an object
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
	CC="gcc -std=gnu11"
	PICFLAG=-fPIC
	OPTMODE=-O3
	;;

    cygwin)
	# cygwin x86_64 gnu
	CC="gcc -std=gnu11"
	PICFLAG=
	OPTMODE=-O3
	;;

    mingw)
	# mingw x86_64 gnu via cygwin
	CC="x86_64-w64-mingw32-gcc -std=gnu11"
	PICFLAG=
	OPTMODE=-O3
	;;

    bsd)
	# 
	CC="clang -std=c11 -fgnu-keywords "
	PICFLAG=
	OPTMODE=-O3
	;;

    *)

	echo "no platform found. i gave up."
	exit 1
	;;
esac


###############################################

$CC $PICFLAG -I. $OPTMODE $* -c

# end of file
