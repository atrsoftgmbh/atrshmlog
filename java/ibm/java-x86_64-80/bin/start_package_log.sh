#!/bin/bash
#!/usr/bin/ksh
# $Id:$
#
# start the ATRSHMLOG test main method

if [ -z $JAVA_JNI_PLATFORM ]
then
    echo "please set the environment with dot.java.sh "
    exit 1
fi


LIBDIR=./de/atrsoft/successorofoak/utilities/logging/atrshmlog/lib

# we use this time the package version. so its with the classspath / package
# the shared lib is also moved to the package lib dir here
# on mingw we need also the runtime dll of the compiler stuff,
# and in case its apthread version the libpthread
# see the doc for more on mingw
#
java -Djava.library.path=$LIBDIR de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOGTest  hallo welt

# end of file
