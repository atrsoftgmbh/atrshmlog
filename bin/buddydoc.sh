#!/bin/bash
#!/usr/local/bin/bash
#!/usr/bin/ksh
#!/bin/ksh
# $Id:$
#
# generate the documentation via doxygen
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
	DOXYGEN=doxygen
	;;

    cygwin)
	# cygwin x86_64 gnu
	DOXYGEN=doxygen
	;;

    mingw)
	# mingw x86_64 gnu via cygwin
	DOXYGEN=
	;;


    bsd)
	case $ATRHMLOG_FLAVOUR in
	    1) # freebsd didn't make it for me. too long timeout
		DOXYGEN=
	    ;;

	    2) # the openbsd did it
		DOXYGEN=doxygen
		;;

	    *)
		DOXYGEN=
		;;
	esac
	
        ;;
    
    *)

	echo "no platform found. i gave up."
	exit 1
	;;
esac

###############################################

$DOXYGEN

# end of file
