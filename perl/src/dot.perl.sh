
# $Id:$
#
#
PERL_INCLUDES="-I$(perl -e 'use Config; print $Config{archlib};')/CORE"
export PERL_INCLUDES

# the perl version 
PERL_VERSION=5
export PERL_VERSION

# we set the platform too
# this is the platform for buildenvironment,: linux, cygwin, mingw, ...
PERL_PLATFORM=linux
export PERL_PLATFORM

PERL_PLATFORM_INCLUDES="-I/usr/include"
export PERL_PLATFORM_INCLUDES

PERL_DEFINES="-D__STDC_VERSION__=201112L  -D__x86_64__=1  -DATRSHMLOG_FLAVOUR=$ATRSHMLOG_FLAVOUR $ATRSHMLOG_PLATFORM_DEFINE "
export PERL_DEFINES

# end of file
