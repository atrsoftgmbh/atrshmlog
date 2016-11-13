#!/bin/bash
#!/usr/bin/ksh
# $Id:$
#
# we copy the jni lib into the target for the test script

date

LIB="$1"

####################################

echo "copy $LIB"

/bin/cp -f $LIB de/atrsoft/successorofoak/utilities/logging/atrshmlog/lib/$LIB

date

# end of file


