#!/bin/bash

if [ $# -le 1 ]
then
    echo "usage : doswig.sh languageflags ... "
    exit 1
fi

SWIGOPTS="-v $* -includeall -MF atrshmlog.depends"
INCLUDES="-I. -I./includes"

swig $SWIGOPTS -o Atrshmlog_wrap.c.$$ $INCLUDES $SWIG_INCLUDE $SWIG_PLATFORM_INCLUDES $SWIG_DEFINES Atrshmlog.i 

# now comes a bad thing.
# the generated file has a flaw because of wrong generated functions parameters
#we correct this right after the generation here

perl -n -e 's:\\\\0:0:; print' Atrshmlog_wrap.c.$$ > Atrshmlog_wrap.c

# we do not delete the original - perhaps we will need it for inspections
