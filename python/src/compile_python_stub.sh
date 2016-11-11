#!/bin/bash
#!/usr/bin/ksh
# $Id:$
#
# we compile the c impl and python bridgecode into a shared lib
#
# usage : compile_python_stub.sh libraryname bridgecode modulecode

date

if [ -z $PYTHON_PLATFORM ]
then
    echo "please set the environment with dot.python.sh "
    exit 1
fi

case $PYTHON_PLATFORM in
    linux)
	# linux x86_64 gnu
	CC="gcc -std=gnu11 -DNDEBUG -Wall"
	PICFLAG=-fPIC
	OPTMODE=-O3
	SHAREMODE=-shared
	;;

    cygwin)
	# cygwin x86_64 gnu
	CC="gcc -std=gnu11 -DNDEBUG -Wall"
	PICFLAG=
	OPTMODE=-O3
	SHAREMODE=-shared
	;;

    mingw)
	# mingw x86_64 gnu via cygwin
	CC="x86_64-w64-mingw32-gcc -std=gnu11 -DNDEBUG -Wall"
	PICFLAG=
	OPTMODE=-O3
	SHAREMODE=-shared
	;;

    *)
	echo "dont know your platform here"
	exit 1
	;;
esac

#####################################
DEST="$1"

shift

# we need the includes and the optimize for inline code here
CFLAGS="$PICFLAG $OPTMODE -I. -I./includes $SHAREMODE"

if [ -n $PYTHON_VERSION ]
then
CFLAGS="$CFLAGS -DATRSHMLOG_PYTHON_VERSION=$PYTHON_VERSION "
fi

if [ -n $PYTHON_PLATFORM ]
then
CFLAGS="$CFLAGS -DATRSHMLOG_PYTHON_PLATFORM=$PYTHON_PLATFORM "
fi

OBJS=""

LIBS=""

while [ "x$1" != "x" ]
do
    if [ $1 != "${1%.c}" ]
    then
	# if we are a .c file we are compiled
	O="$(basename $1 \.c).o"
	/bin/rm -f $O
	echo "compile $1"
	$CC $CFLAGS "-I${PYTHON_INCLUDE}" -c -o $O $1
	OBJS="$OBJS $O"
    else
	# no, we are no .c, so we are a lib 
	LIBS="$LIBS $1"
    fi
    
    shift 
done

# now we link
echo "link $DEST for $OBJS and $LIBS"
$CC $SHAREMODE -L. $OBJS $LIBS -o $DEST

date

# end of file
