#
# $Id:$
#
# the ruby dot file

RUBY_INCLUDE=/usr/include
export RUBY_INCLUDE

# the python version only 2 or 3 for now
RUBY_VERSION=2
export RUBY_VERSION

# we set the platform too
# this is the platform for buildenvironment,: linux, cygwin, mingw, ...
RUBY_PLATFORM=linux
export RUBY_PLATFORM

# we set the path to find the module for tests
RUBYPATH=.:$(pwd)
export RUBYPATH


######################################################################

if [ -z $RUBY_PLATFORM ]
then
    echo "please set the environment with dot.ruby.sh "
    exit 1
fi

case $RUBY_PLATFORM in
    linux)
	# linux x86_64 gnu
	CC="gcc -std=gnu11 -DNDEBUG -Wall  -DATRSHMLOG_FLAVOUR=$ATRSHMLOG_FLAVOUR $ATRSHMLOG_PLATFORM_DEFINE -O3 -fPIC"
	;;

    cygwin)
	# cygwin x86_64 gnu
	CC="gcc -std=gnu11 -DNDEBUG -Wall  -DATRSHMLOG_FLAVOUR=$ATRSHMLOG_FLAVOUR $ATRSHMLOG_PLATFORM_DEFINE -O3 "
	;;

    mingw)
	# mingw x86_64 gnu via cygwin
	CC="x86_64-w64-mingw32-gcc -std=gnu11 -DNDEBUG -Wall  -DATRSHMLOG_FLAVOUR=$ATRSHMLOG_FLAVOUR $ATRSHMLOG_PLATFORM_DEFINE -O3 "
	;;

    *)
	echo "dont know your platform here"
	exit 1
	;;
esac

export CC

# end of file
