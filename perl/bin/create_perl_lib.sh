#!/bin/bash
#!/usr/bin/ksh
# $Id:$
#
# we create the perl lib.
# we make the class files, the create the header,
# and then compile the lib and copy it
# this is done simple, so it can give some errors..


date


if [ -z $PERL_PLATFORM ]
then
    echo "please set the environment with dot.perl.sh "
    exit 1
fi

case $PERL_PLATFORM in
    linux)
	# linux x86_64 gnu
	LIB=Atrshmlog.so
	SOURCES="Atrshmlog_wrap.c atrshmlog_perlwrapper.c"
	LIBS=-latrshmlog
	;;

    cygwin)
	# cygwin x86_64 gnu
	LIB=Atrshmlog.dll
	SOURCES="Atrshmlog_wrap.c atrshmlog_perlwrapper.c"
	LIBS=-latrshmlog
	;;

    mingw)
	# mingw x86_64 gnu via cygwin
	LIB=atrshmlog.dll
	SOURCES="Atrshmlog_wrap.c atrshmlog_perlwrapper.c"
	LIBS=-latrshmlog
	;;

    posix)
	# linux x86_64 gnu
	LIB=atrshmlog.so
	SOURCES="Atrshmlog_wrap.c atrshmlog_perlwrapper.c"
	LIBS=-latrshmlog
	;;
	
    *)
	echo "dont know your platform here"
	exit 1
	;;
esac


####################################

doswig.sh
./compile_perl_stub.sh $LIB $SOURCES $LIBS
 
date

# end of file


