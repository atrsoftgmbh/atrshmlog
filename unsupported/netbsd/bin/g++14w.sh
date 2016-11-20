#!/bin/ksh
#!/bin/bash
#!/usr/local/bin/bash
#!/usr/bin/ksh
#!/bin/ksh
#
# $Id:$
#
# Compile the c++ thing and link it
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
		CPP="g++ -std=c++14 -pthread -Wall -Weffc++ -fdump-tree-original  -DATRSHMLOG_FLAVOUR=$ATRSHMLOG_FLAVOUR "
		PICFLAG=-fPIC
		OPTMODE=-O3
		LIBMODULE=-latrshmlog
		;;

	    2) # centos 7.2 with a somewhat outdated g++
		CPP="g++ -std=gnu++1y -pthread -Wall -Weffc++ -fdump-tree-original"
		PICFLAG=-fPIC
		OPTMODE=-O3
		LIBMODULE=-latrshmlog
		;;

	    *)
		# linux x86_64 gnu
		CPP="g++ -std=c++14 -pthread -Wall -Weffc++ -fdump-tree-original  -DATRSHMLOG_FLAVOUR=$ATRSHMLOG_FLAVOUR "
		PICFLAG=-fPIC
		OPTMODE=-O3
		LIBMODULE=-latrshmlog
		;;
	esac

	;;

    cygwin)
	# cygwin x86_64 gnu
	CPP="g++ -std=c++14 -pthread -Wall -Weffc++ -fdump-tree-original"
	PICFLAG=
	OPTMODE=-O3
	LIBMODULE=-latrshmlog
	;;

    mingw)
	# mingw x86_64 gnu via cygwin 
	CPP="x86_64-w64-mingw32-g++ -std=c++14 -pthread -Wall -Weffc++ -fdump-tree-original"
	PICFLAG= 
	OPTMODE=-O3
	LIBMODULE=-latrshmlog
	;;

    bsd)
	case $ATRSHMLOG_FLAVOUR in
	    3)
		# 
		CPP="clang++ -std=c++14  -DATRSHMLOG_FLAVOUR=$ATRSHMLOG_FLAVOUR "
		PICFLAG=
		OPTMODE=-O3
		LIBMODULE="-latrshmlog  -lpthread"
		;;

	    4)
		CPP="clang++ -std=c++14  -DATRSHMLOG_FLAVOUR=$ATRSHMLOG_FLAVOUR "
		PICFLAG=
		OPTMODE=-O3
		LIBMODULE="-latrshmlog -lpthread"
		;;

	    5)
		CPP="g++ -std=gnu++14  -DATRSHMLOG_FLAVOUR=$ATRSHMLOG_FLAVOUR "
		PICFLAG=-fPIC
		OPTMODE=-O3
		LIBMODULE="-latrshmlog -lpthread"
		;;
	    
	    *)
		CPP="CC "
		PICFLAG=
		OPTMODE=-O3
		LIBMODULE="-latrshmlog "
		;;

	esac
	;;
		

    *)

	echo "no platform found. i gave up."
	exit 1
	;;
esac






#############################################

INFILE=${1%%.cpp}
INFILE=${INFILE%%.C}
INFILE=${INFILE%%.c++}
INFILE=${INFILE}.C

OUTFILE=${INFILE%%.C}

$CPP $PICFLAG -L. $OPTMODE "${INFILE}" $LIBMODULE -o "$OUTFILE"

# end of file

