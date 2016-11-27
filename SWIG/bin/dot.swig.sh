
# $Id:$
#
#
SWIG_INCLUDES="-I."
export SWIG_INCLUDES

# the version 
SWIG_VERSION=3
export SWIG_VERSION

# we set the platform too
# this is the platform for buildenvironment,: linux, cygwin, mingw, ...
SWIG_PLATFORM=linux
export SWIG_PLATFORM

SWIG_PLATFORM_INCLUDES="-I/usr/include"
export SWIG_PLATFORM_INCLUDES

SWIG_DEFINES="-D__STDC_VERSION__=201112L  -D__x86_64__=1  -DATRSHMLOG_FLAVOUR=$ATRSHMLOG_FLAVOUR $ATRSHMLOG_PLATFORM_DEFINE "
export SWIG_DEFINES

# end of file
