#!/usr/local/bin/bash
#!/usr/bin/ksh
# $Id:$
#
# build the lib
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
	AR=ar
	LIB=libatrshmlog.a
	OBJ="atrshmlog.o $(ls impls/*.o | grep atrshmlogimpl )"
	;;

    cygwin)
	# cygwin x86_64 gnu
	AR=ar
	LIB=libatrshmlog.a
	OBJ="atrshmlog.o $(ls impls/*.o | grep atrshmlogimpl )"
	;;

    mingw)
	# mingw x86_64 gnu via cygwin
	AR="x86_64-w64-mingw32-ar"
	LIB=libatrshmlog.a
	OBJ="atrshmlog.o $(ls impls/*.o | grep atrshmlogimpl )"
	;;


    bsd)
	# 
	AR=ar
	LIB=libatrshmlog.a
	OBJ="atrshmlog.o $(ls impls/*.o | grep atrshmlogimpl )"
	;;

    *)

	echo "no platform found. i gave up."
	exit 1
	;;
esac

##############################################

if [ -e $LIB ]
then
    rm $LIB
fi

$AR r $LIB $OBJ
$AR s $LIB

# end of file
