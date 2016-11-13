#!/bin/bash
#!/usr/bin/ksh
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
	# linux x86_64 gnu
	CPP="g++ -std=c++14 -pthread -Wall -Weffc++ -fdump-tree-original"
	PICFLAG=-fPIC
	OPTMODE=-O3
	LIBMODULE=-latrshmlog
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

