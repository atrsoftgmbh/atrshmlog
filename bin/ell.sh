#!/bin/bash
#!/usr/local/bin/bash
#!/usr/bin/ksh
#!/bin/ksh
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
	case $ATRSHMLOG_FLAVOUR in
	    1) # fedora way
		# linux x86_64 gnu
		CC="gcc -pthread"
		LIBMODULE=-latrshmlog
		;;

	    2) # centos 7.2 after manual compile of gcc 5.4
		CC="x86_64-unknown-linux-gnu-gcc-5.4.0 -pthread"
		LIBMODULE=-latrshmlog
		;;

	    7) # ubuntu way
		# linux x86_64 gnu
		CC="gcc -pthread"
		LIBMODULE=-latrshmlog
		;;
		
	    8) # opensuse
		# linux x86_64 gnu
		CC="gcc-6 -pthread"
		LIBMODULE=-latrshmlog
		;;
		
	    9) # debian 8.6 
		# linux x86_64 gnu
		CC="gcc -pthread"
		LIBMODULE=-latrshmlog
		;;

	    10) # sles , gcc 5.4.0 from source
		# linux x86_64 gnu
		CC="gcc -pthread"
		LIBMODULE=-latrshmlog
		;;
	    
	    *)
		# linux x86_64 gnu
		CC="gcc -pthread"
		LIBMODULE=-latrshmlog
		;;
	esac

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

    bsd)
	case $ATRSHMLOG_FLAVOUR in
	    3)
		#  freebsd
		CC="clang "
		LIBMODULE="-latrshmlog  -lpthread"
		;;
	    4)
		# openbsd
		CC="clang "
		LIBMODULE="-latrshmlog -lpthread"
		;;

	    
	    5) # netbsd has a gcc 6.2 in /usr/pkg/gcc6/bin
		CC="gcc -pthread"
		LIBMODULE=-latrshmlog
		;;

	    *)
		CC=cc
		LIBMODULE=-latrshmlog
		;;
	esac
	;;

    solaris)
	CC="gcc -pthread"
	LIBMODULE="-latrshmlog -lthread -lpthread"
	;;

    *)

	echo "no platform found. i gave up."
	exit 1
	;;
esac








###############################################

P=${1%%.o}

shift

$CC $LDFLAGS -L. -L.. ${P}.o $* $LIBMODULE -o $P

# end of file
