#!/bin/bash
#!/usr/local/bin/bash
#!/usr/bin/ksh
#!/bin/ksh
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
	case $ATRSHMLOG_FLAVOUR in
	    1) # fedora
		# linux x86_64 gnu
		CC="gcc -std=gnu11 -DATRSHMLOG_FLAVOUR=$ATRSHMLOG_FLAVOUR "
		PICFLAG=-fPIC
		OPTMODE=-O3
		;;

	    2) # centos 7.2 with manual 5.4 gcc
		CC="x86_64-unknown-linux-gnu-gcc-5.4.0 -std=gnu11 -DATRSHMLOG_FLAVOUR=$ATRSHMLOG_FLAVOUR "
		PICFLAG=-fPIC
		OPTMODE=-O3
		;;

	    *)
		# linux x86_64 gnu
		CC="gcc -std=gnu11 -DATRSHMLOG_FLAVOUR=$ATRSHMLOG_FLAVOUR "
		PICFLAG=-fPIC
		OPTMODE=-O3
		;;

	esac

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
	case $ATRSHMLOG_FLAVOUR in
	    1)
		# 
		CC="clang -std=c11 -fgnu-keywords  -DATRSHMLOG_FLAVOUR=$ATRSHMLOG_FLAVOUR "
		PICFLAG=
		OPTMODE=-O3
		;;

	   2)
		# 
		CC="clang -std=c11 -fgnu-keywords  -DATRSHMLOG_FLAVOUR=$ATRSHMLOG_FLAVOUR "
		PICFLAG=
		OPTMODE=-O3
		;;

	    *)
		# 
		CC="cc "
		PICFLAG=
		OPTMODE=-O3
		;;

	esac
	;;

		

    *)

	echo "no platform found. i gave up."
	exit 1
	;;
esac


###############################################

$CC $PICFLAG -I. $OPTMODE $* -c

# end of file
