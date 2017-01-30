#!/bin/bash
#!/usr/bin/ksh
# $Id:$
#
# we create the jni lib.
# we make the class files, the create the header,
# and then compile the lib and copy it
# this is done simple, so it can give some errors..


date


if [ -z $PYTHON_PLATFORM ]
then
    echo "please set the environment with dot.python.sh "
    exit 1
fi

case $PYTHON_PLATFORM in
    linux)
	# linux x86_64 gnu
	LIB=atrshmlog.so
	SOURCES="atrshmlogpython.c"
	LIBS=-latrshmlog
	;;

    cygwin)
	# cygwin x86_64 gnu
	LIB=atrshmlog.dll
	SOURCES="atrshmlogpython.c"
	LIBS=-latrshmlog
	;;

    mingw)
	# mingw x86_64 gnu via cygwin
	LIB=atrshmlog.dll
	SOURCES="atrshmlogpython.c"
	LIBS=-latrshmlog
	;;

    posix)
	# linux x86_64 gnu
	LIB=atrshmlog.so
	SOURCES="atrshmlogpython.c"
	LIBS=-latrshmlog
	;;
	
    *)
	echo "dont know your platform here"
	exit 1
	;;
esac


####################################

./compile_python_stub.sh $LIB $SOURCES $LIBS
 
date

# end of file


