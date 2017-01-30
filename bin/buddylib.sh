#!/bin/bash
#!/usr/local/bin/bash
#!/usr/bin/ksh
#!/bin/ksh
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
	LIB=libatrshmlogc.a
	OBJ="atrshmlogc.o $(ls impls/*.o | grep atrshmlogimpl )"
	REBUILDTABLE="s"
	;;

    cygwin)
	# cygwin x86_64 gnu
	AR=ar
	LIB=libatrshmlogc.a
	OBJ="atrshmlogc.o $(ls impls/*.o | grep atrshmlogimpl )"
	REBUILDTABLE="s"
	;;

    mingw)
	# mingw x86_64 gnu via cygwin
	AR="x86_64-w64-mingw32-ar"
	LIB=libatrshmlogc.a
	OBJ="atrshmlogc.o $(ls impls/*.o | grep atrshmlogimpl )"
	REBUILDTABLE="s"
	;;


    bsd)
	# 
	AR=ar
	LIB=libatrshmlogc.a
	OBJ="atrshmlogc.o $(ls impls/*.o | grep atrshmlogimpl )"
	REBUILDTABLE="s"
	;;

    solaris)
	# 
	AR=ar
	LIB=libatrshmlogc.a
	OBJ="atrshmlogc.o $(ls impls/*.o | grep atrshmlogimpl )"
	REBUILDTABLE="-r -s"
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

$AR $ARFLAGS r $LIB $OBJ
$AR $REBUILDTABLE $LIB

# end of file
